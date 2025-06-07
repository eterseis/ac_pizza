#pragma once
#include <cstdint>

namespace Offsets
{
	/* Entity struct */
	inline constexpr uintptr_t head{ 0x4 };			/* VECTOR3  */
	inline constexpr uintptr_t feet{ 0x28 };		/* VECTOR3  */

	inline constexpr uintptr_t yaw{ 0x34 };			/* FLOAT	*/
	inline constexpr uintptr_t pitch{ 0x38 };		/* FLOAT	*/

	inline constexpr uintptr_t health{ 0xEC };		/* INT		*/
	inline constexpr uintptr_t ammo{ 0x140 };		/* INT		*/
	inline constexpr uintptr_t rapidfire{ 0x164 };  /* FLOAT	*/
	inline constexpr uintptr_t name{ 0x205 };		/* CHAR[16] */
	inline constexpr uintptr_t team{ 0x30C };		/* INT		*/

	/* Game */
	inline constexpr uintptr_t localplayer{ 0x17E0A8 };
	inline constexpr uintptr_t entity_list{ 0x191FCC };		/* ENTITY ADDRESS ARRAY */
	inline constexpr uintptr_t living_entities{ 0x18AC0C }; /* INT */
	inline constexpr uintptr_t view_matrix{ /* ac_client.exe + */ 0x17DFFC };

	uintptr_t GetEntityList();
	uintptr_t GetLocalPlayer();
	unsigned int GetLivingEntities();
	uintptr_t GetViewMatrix();
}