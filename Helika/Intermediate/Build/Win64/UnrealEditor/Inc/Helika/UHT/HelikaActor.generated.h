// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "HelikaActor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FHSession;
#ifdef HELIKA_HelikaActor_generated_h
#error "HelikaActor.generated.h already included, missing '#pragma once' in HelikaActor.h"
#endif
#define HELIKA_HelikaActor_generated_h

#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_28_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FHEvent_Statics; \
	HELIKA_API static class UScriptStruct* StaticStruct();


template<> HELIKA_API UScriptStruct* StaticStruct<struct FHEvent>();

#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_46_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FHSession_Statics; \
	HELIKA_API static class UScriptStruct* StaticStruct();


template<> HELIKA_API UScriptStruct* StaticStruct<struct FHSession>();

#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_SPARSE_DATA
#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSendEvent);


#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_ACCESSORS
#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAHelikaActor(); \
	friend struct Z_Construct_UClass_AHelikaActor_Statics; \
public: \
	DECLARE_CLASS(AHelikaActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Helika"), NO_API) \
	DECLARE_SERIALIZER(AHelikaActor)


#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AHelikaActor(AHelikaActor&&); \
	NO_API AHelikaActor(const AHelikaActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AHelikaActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AHelikaActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AHelikaActor) \
	NO_API virtual ~AHelikaActor();


#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_55_PROLOG
#define FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_SPARSE_DATA \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_ACCESSORS \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_INCLASS_NO_PURE_DECLS \
	FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_58_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> HELIKA_API UClass* StaticClass<class AHelikaActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Helika_helika_ue_sdk_Helika_HostProject_Plugins_Helika_Source_Helika_Public_HelikaActor_h


#define FOREACH_ENUM_HELIKAENVIRONMENT(op) \
	op(HelikaEnvironment::Localhost) \
	op(HelikaEnvironment::Develop) \
	op(HelikaEnvironment::Production) 

enum class HelikaEnvironment : uint8;
template<> struct TIsUEnumClass<HelikaEnvironment> { enum { Value = true }; };
template<> HELIKA_API UEnum* StaticEnum<HelikaEnvironment>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
