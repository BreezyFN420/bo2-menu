#include "includes.h"
#include "veh.h"
#include <chrono>
#include "engine.h"
#include "hwbp.h"
#include "gui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

bool g_ShowMenu = true;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
	{
		g_ShowMenu = !g_ShowMenu;
		ImGui::GetIO().MouseDrawCursor = g_ShowMenu;
	}

	if (g_ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

static bool wallhack_was_enabled = false;
std::atomic<bool> wallhack_active{ false };
std::atomic<bool> chams_active{ false };

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (g_ShowMenu)
		gui.display();

	auto now = std::chrono::steady_clock::now();
	static auto last_toggle = std::chrono::steady_clock::now();
	static auto last_refresh = std::chrono::steady_clock::now();
	// This is to prevent race conditions when toggled ON/OFF.

	bool wallhack_enabled = settings.visuals.toggle_wallhack;

	if (wallhack_enabled != wallhack_was_enabled)
	{
		if (now - last_toggle > std::chrono::milliseconds(30))
		{
			if (wallhack_enabled)
			{
				wallhack_active.store(true, std::memory_order_release);
				chams_active.store(true, std::memory_order_release);
				hwbp::SetAll(reinterpret_cast<void*>(0x7269C0), 0);
				hwbp::SetAll(reinterpret_cast<void*>(0x728981), 1);
			}
			else
			{
				hwbp::ClearAll(0);
				hwbp::ClearAll(1);
				wallhack_active.store(false, std::memory_order_release);
				chams_active.store(false, std::memory_order_release);
			}

			wallhack_was_enabled = wallhack_enabled;
			last_toggle = now;
		}
	}

	// Because the hardware breakpoint hooks are unstable (which causes chams to flicker and wallhack to get removed), 
	// I've opted to set it every 30 seconds when you're in-game. This causes performance issues.
	if (wallhack_active.load(std::memory_order_acquire))
	{
		if (now - last_refresh > std::chrono::seconds(30))
		{
			hwbp::SetAll(reinterpret_cast<void*>(0x7269C0), 0);		// wallhack
			hwbp::SetAll(reinterpret_cast<void*>(0x728981), 1);		// chams
			last_refresh = now;
		}
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID reserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	engine.init();
	veh.init();

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE instance, DWORD call_reason, LPVOID reserved)
{
	switch (call_reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(instance);
		CreateThread(nullptr, 0, MainThread, instance, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}