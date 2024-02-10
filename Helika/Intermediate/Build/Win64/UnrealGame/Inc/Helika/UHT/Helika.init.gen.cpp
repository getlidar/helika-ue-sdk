// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHelika_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Helika;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Helika()
	{
		if (!Z_Registration_Info_UPackage__Script_Helika.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Helika",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xD63042DC,
				0xF66936AF,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Helika.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Helika.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Helika(Z_Construct_UPackage__Script_Helika, TEXT("/Script/Helika"), Z_Registration_Info_UPackage__Script_Helika, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xD63042DC, 0xF66936AF));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
