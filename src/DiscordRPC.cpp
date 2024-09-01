#include "DiscordRPC.h"

#include "Memory.h"
#include "Log.h"

#include <chrono>
#include <thread>

discord::Core* core{};
auto result = discord::Core::Create(1279839098848411738, DiscordCreateFlags_Default, &core);

discord::Activity activity{};

void DiscordRPCThread()
{
    SystemLog->Out(activity.GetName());
    activity.SetName("bzone RPC test");
    SystemLog->Out(activity.GetName());
    activity.SetDetails("Testing");
    activity.SetState("Vibing");
    activity.SetType(discord::ActivityType::Playing);
    core->ActivityManager().UpdateActivity(activity, NULL);
	while (true)
	{
        if (Memory::CheckExitCondition(5, "Exit condition detected, exiting Discord RPC thread"))
        {
            core->ActivityManager().ClearActivity(NULL);
            break;
        }




        core->RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}