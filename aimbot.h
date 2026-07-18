#include "aimbot.h"

int aim_key = VK_RBUTTON;
int target_number = -1;
float last_distance = FLT_MAX;
vec3_t aim_angles;

// Akolinnn (unknowncheats): The process is the same in every CoD game.
// 1.Grab the entity list(as efk mentioned above).
// 2.Filter the entities(alive, visible, closest, fov etc).
// 3.Use CG_DObjGetWorldTagPos to get the desired aimbot position.
// 4.Subtract your vieworigin from the targeted angles
// 5.Use vectoangles
// 6.Subtract your delta angles from the targeted angles				(playerState->deltaAngles)
// 7.And finally set your current viewangles to the targeted angles.	(clientActive->viewAngles)

void features::standard_aim()
{
	if (!settings.features.toggle_aimbot)
		return;

	if (!functions::CL_LocalClientIsInGame())
		return;

	if (!GetAsyncKeyState(aim_key))
		return;

	vec3_t viewOrigin;
	functions::CG_GetPlayerViewOrigin(&viewOrigin);

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		// 1 & 2. Grab the entity list and make sure it's valid. Filter by entity type, team,...
		centity_t* entity = &cg_entities[i];
		
		if (!entity || !(entity->IsAlive & 2) || entity->eType != ET_PLAYER)
			continue;

		if (functions::EntityIsTeammate(entity))
			continue;

		// 3. Get the desired aimbot position (= which player tag/bone you want to aim at)
		vec3_t head_pos;
		WORD head_tag = functions::RegisterTag("j_head", 7);

		vec3_t screen_pos; // (for w2s)
		if (functions::GetPlayerTag(entity, head_tag, head_pos) && functions::CG_WorldPosToScreenPos(head_pos, screen_pos))
		{
			if (!functions::AimTarget_IsTargetVisible(entity, head_tag))
				continue;

			// 2. (cont) For this aimbot we are going to be filtering by distance (essentially getting the smallest element in an array).
			// It's also possible to filter by FOV (closest to your crosshair within a certain radius) or health (target the weakest entities first)
			float new_distance = GetDistance3D(viewOrigin, head_pos);
			if (new_distance < last_distance)
			{
				last_distance = new_distance;
				target_number = i;
			}

			// Set our angles to the latest target. For snap aim, we have to use clientActive viewangles. silent aim uses usercmd viewangles
			if (target_number != -1)
			{
				auto target = cg_entities[target_number];

				// 4 & 5. Subtract your vieworigin from the targeted angles. Use vectoangles
				float* direction_vector = GetDistance(head_pos, viewOrigin);
				vectoangles(direction_vector, aim_angles);

				// 6. Subtract your delta angles from the targeted angles
				aim_angles[0] -= cg->playerState.vDeltaAngles[0];
				aim_angles[1] -= cg->playerState.vDeltaAngles[1];

				// 7. Set your current viewangles to the targeted angles.
				clientActive->viewangles[0] = aim_angles[0];
				clientActive->viewangles[1] = aim_angles[1];
			}
		}
	}
}

void features::silent_aim()
{
	// ...
}