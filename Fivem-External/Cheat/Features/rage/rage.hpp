#pragma once

#include "../../FivemSDK/Fivem.hpp"

namespace Cheat
{
	class CFeatureHandler
	{
	private:
		bool CurrentState = false;
		bool Once = false;

	public:
		void HandleFeature(bool IsEnabled, std::function< void(bool) > Function)
		{
			if (IsEnabled && !CurrentState)
			{
				Function(true);
				CurrentState = true;
			}
			else if (!IsEnabled && CurrentState)
			{
				Function(false);
				CurrentState = false;
			}
		}

		void HandleFeature(bool IsEnabled, float Amount, std::function< void() > Function)
		{
			static bool OldBool = false;
			static float OldFloat = 1.f;

			if (IsEnabled != OldBool || Amount != OldFloat || !CurrentState)
			{
				Function();

				CurrentState = true;

				OldBool = IsEnabled;
				OldFloat = Amount;
			}
		}

		void ReEnableFeature()
		{
			CurrentState = false;
		}

		bool IsEnabled() const
		{
			return CurrentState;
		}
	};


	namespace Exploits
	{
		void TeleportToWaypoint();
		void RunThread();
		void Revive();
		void FullArmor();
		void Suicide();
		void UnlockClosestVehicle();
	}
}