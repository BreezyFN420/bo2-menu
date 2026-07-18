#include "engine.h"

cg_t* cg;
centity_t* cg_entities;
clientActive_t* clientActive;

bool engine_c::init() noexcept
{
	cg = *(cg_t**)0x113F18C;
	cg_entities = *(centity_t**)0x1140878;
	clientActive = *(clientActive_t**)0x11C7D74;

	// playerstate, refdef: use cg->playerState and cg->refdef

	return true;
}
