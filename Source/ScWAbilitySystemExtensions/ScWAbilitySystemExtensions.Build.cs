// Scientific Ways

using UnrealBuildTool;
using System.IO; // for Path

public class ScWAbilitySystemExtensions : ModuleRules
{
	public ScWAbilitySystemExtensions(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{

		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",

            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",

            "ScWGameCore",
        });

		PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreUObject",
            "Engine",

            "NavigationSystem",
        });
	}
}
