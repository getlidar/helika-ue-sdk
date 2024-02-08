// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Helika/Public/HelikaActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHelikaActor() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	HELIKA_API UClass* Z_Construct_UClass_AHelikaActor();
	HELIKA_API UClass* Z_Construct_UClass_AHelikaActor_NoRegister();
	HELIKA_API UEnum* Z_Construct_UEnum_Helika_HelikaEnvironment();
	HELIKA_API UScriptStruct* Z_Construct_UScriptStruct_FHEvents();
	HELIKA_API UScriptStruct* Z_Construct_UScriptStruct_FHSession();
	UPackage* Z_Construct_UPackage__Script_Helika();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_HelikaEnvironment;
	static UEnum* HelikaEnvironment_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_HelikaEnvironment.OuterSingleton)
		{
			Z_Registration_Info_UEnum_HelikaEnvironment.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_Helika_HelikaEnvironment, (UObject*)Z_Construct_UPackage__Script_Helika(), TEXT("HelikaEnvironment"));
		}
		return Z_Registration_Info_UEnum_HelikaEnvironment.OuterSingleton;
	}
	template<> HELIKA_API UEnum* StaticEnum<HelikaEnvironment>()
	{
		return HelikaEnvironment_StaticEnum();
	}
	struct Z_Construct_UEnum_Helika_HelikaEnvironment_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::Enumerators[] = {
		{ "HelikaEnvironment::Localhost", (int64)HelikaEnvironment::Localhost },
		{ "HelikaEnvironment::Develop", (int64)HelikaEnvironment::Develop },
		{ "HelikaEnvironment::Production", (int64)HelikaEnvironment::Production },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Develop.Name", "HelikaEnvironment::Develop" },
		{ "Localhost.Name", "HelikaEnvironment::Localhost" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
		{ "Production.Name", "HelikaEnvironment::Production" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_Helika,
		nullptr,
		"HelikaEnvironment",
		"HelikaEnvironment",
		Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::Enum_MetaDataParams), Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_Helika_HelikaEnvironment()
	{
		if (!Z_Registration_Info_UEnum_HelikaEnvironment.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_HelikaEnvironment.InnerSingleton, Z_Construct_UEnum_Helika_HelikaEnvironment_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_HelikaEnvironment.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_HEvents;
class UScriptStruct* FHEvents::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_HEvents.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_HEvents.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FHEvents, (UObject*)Z_Construct_UPackage__Script_Helika(), TEXT("HEvents"));
	}
	return Z_Registration_Info_UScriptStruct_HEvents.OuterSingleton;
}
template<> HELIKA_API UScriptStruct* StaticStruct<FHEvents>()
{
	return FHEvents::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FHEvents_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_game_id_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_game_id;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_event_type_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_event_type;
		static const UECodeGen_Private::FStrPropertyParams NewProp_event_ValueProp;
		static const UECodeGen_Private::FStrPropertyParams NewProp_event_Key_KeyProp;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_event_MetaData[];
#endif
		static const UECodeGen_Private::FMapPropertyParams NewProp_event;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_created_at_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_created_at;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHEvents_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FHEvents_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FHEvents>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_game_id_MetaData[] = {
		{ "Category", "HEvents" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_game_id = { "game_id", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FHEvents, game_id), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_game_id_MetaData), Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_game_id_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_type_MetaData[] = {
		{ "Category", "HEvents" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_type = { "event_type", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FHEvents, event_type), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_type_MetaData), Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_type_MetaData) };
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_ValueProp = { "event", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_Key_KeyProp = { "event_Key", nullptr, (EPropertyFlags)0x0000000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_MetaData[] = {
		{ "Category", "HEvents" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event = { "event", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FHEvents, event), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_MetaData), Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_created_at_MetaData[] = {
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_created_at = { "created_at", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FHEvents, created_at), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_created_at_MetaData), Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_created_at_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FHEvents_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_game_id,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_type,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_ValueProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event_Key_KeyProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_event,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHEvents_Statics::NewProp_created_at,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FHEvents_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_Helika,
		nullptr,
		&NewStructOps,
		"HEvents",
		Z_Construct_UScriptStruct_FHEvents_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::PropPointers),
		sizeof(FHEvents),
		alignof(FHEvents),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FHEvents_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHEvents_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FHEvents()
	{
		if (!Z_Registration_Info_UScriptStruct_HEvents.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_HEvents.InnerSingleton, Z_Construct_UScriptStruct_FHEvents_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_HEvents.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_HSession;
class UScriptStruct* FHSession::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_HSession.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_HSession.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FHSession, (UObject*)Z_Construct_UPackage__Script_Helika(), TEXT("HSession"));
	}
	return Z_Registration_Info_UScriptStruct_HSession.OuterSingleton;
}
template<> HELIKA_API UScriptStruct* StaticStruct<FHSession>()
{
	return FHSession::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FHSession_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_id_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_id;
		static const UECodeGen_Private::FStructPropertyParams NewProp_events_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_events_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_events;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHSession_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FHSession_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FHSession>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHSession_Statics::NewProp_id_MetaData[] = {
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FHSession_Statics::NewProp_id = { "id", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FHSession, id), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHSession_Statics::NewProp_id_MetaData), Z_Construct_UScriptStruct_FHSession_Statics::NewProp_id_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events_Inner = { "events", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FHEvents, METADATA_PARAMS(0, nullptr) }; // 1766944434
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events_MetaData[] = {
		{ "Category", "HSession" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events = { "events", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FHSession, events), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events_MetaData), Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events_MetaData) }; // 1766944434
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FHSession_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHSession_Statics::NewProp_id,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FHSession_Statics::NewProp_events,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FHSession_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_Helika,
		nullptr,
		&NewStructOps,
		"HSession",
		Z_Construct_UScriptStruct_FHSession_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHSession_Statics::PropPointers),
		sizeof(FHSession),
		alignof(FHSession),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHSession_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FHSession_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FHSession_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FHSession()
	{
		if (!Z_Registration_Info_UScriptStruct_HSession.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_HSession.InnerSingleton, Z_Construct_UScriptStruct_FHSession_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_HSession.InnerSingleton;
	}
	DEFINE_FUNCTION(AHelikaActor::execSendEvent)
	{
		P_GET_STRUCT(FHSession,Z_Param_helikaEvents);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SendEvent(Z_Param_helikaEvents);
		P_NATIVE_END;
	}
	void AHelikaActor::StaticRegisterNativesAHelikaActor()
	{
		UClass* Class = AHelikaActor::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SendEvent", &AHelikaActor::execSendEvent },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AHelikaActor_SendEvent_Statics
	{
		struct HelikaActor_eventSendEvent_Parms
		{
			FHSession helikaEvents;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_helikaEvents;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::NewProp_helikaEvents = { "helikaEvents", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(HelikaActor_eventSendEvent_Parms, helikaEvents), Z_Construct_UScriptStruct_FHSession, METADATA_PARAMS(0, nullptr) }; // 3048710806
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::NewProp_helikaEvents,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::Function_MetaDataParams[] = {
		{ "Category", "Helika" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AHelikaActor, nullptr, "SendEvent", nullptr, nullptr, Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::PropPointers), sizeof(Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::HelikaActor_eventSendEvent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::Function_MetaDataParams), Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::HelikaActor_eventSendEvent_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_AHelikaActor_SendEvent()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AHelikaActor_SendEvent_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AHelikaActor);
	UClass* Z_Construct_UClass_AHelikaActor_NoRegister()
	{
		return AHelikaActor::StaticClass();
	}
	struct Z_Construct_UClass_AHelikaActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_apiKey_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_apiKey;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_gameId_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_gameId;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_playerId_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_playerId;
		static const UECodeGen_Private::FBytePropertyParams NewProp_helikaEnv_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_helikaEnv_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_helikaEnv;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_sendingEvents_MetaData[];
#endif
		static void NewProp_sendingEvents_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_sendingEvents;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AHelikaActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Helika,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_AHelikaActor_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AHelikaActor_SendEvent, "SendEvent" }, // 3188119904
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHelikaActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "HelikaActor.h" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHelikaActor_Statics::NewProp_apiKey_MetaData[] = {
		{ "Category", "Helika" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_AHelikaActor_Statics::NewProp_apiKey = { "apiKey", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHelikaActor, apiKey), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::NewProp_apiKey_MetaData), Z_Construct_UClass_AHelikaActor_Statics::NewProp_apiKey_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHelikaActor_Statics::NewProp_gameId_MetaData[] = {
		{ "Category", "Helika" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_AHelikaActor_Statics::NewProp_gameId = { "gameId", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHelikaActor, gameId), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::NewProp_gameId_MetaData), Z_Construct_UClass_AHelikaActor_Statics::NewProp_gameId_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHelikaActor_Statics::NewProp_playerId_MetaData[] = {
		{ "Category", "Helika" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_AHelikaActor_Statics::NewProp_playerId = { "playerId", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHelikaActor, playerId), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::NewProp_playerId_MetaData), Z_Construct_UClass_AHelikaActor_Statics::NewProp_playerId_MetaData) };
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv_MetaData[] = {
		{ "Category", "Helika" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv = { "helikaEnv", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AHelikaActor, helikaEnv), Z_Construct_UEnum_Helika_HelikaEnvironment, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv_MetaData), Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv_MetaData) }; // 922295950
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents_MetaData[] = {
		{ "Category", "Helika" },
		{ "ModuleRelativePath", "Public/HelikaActor.h" },
	};
#endif
	void Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents_SetBit(void* Obj)
	{
		((AHelikaActor*)Obj)->sendingEvents = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents = { "sendingEvents", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AHelikaActor), &Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents_MetaData), Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AHelikaActor_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHelikaActor_Statics::NewProp_apiKey,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHelikaActor_Statics::NewProp_gameId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHelikaActor_Statics::NewProp_playerId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHelikaActor_Statics::NewProp_helikaEnv,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHelikaActor_Statics::NewProp_sendingEvents,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AHelikaActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AHelikaActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AHelikaActor_Statics::ClassParams = {
		&AHelikaActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AHelikaActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::Class_MetaDataParams), Z_Construct_UClass_AHelikaActor_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AHelikaActor_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_AHelikaActor()
	{
		if (!Z_Registration_Info_UClass_AHelikaActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AHelikaActor.OuterSingleton, Z_Construct_UClass_AHelikaActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AHelikaActor.OuterSingleton;
	}
	template<> HELIKA_API UClass* StaticClass<AHelikaActor>()
	{
		return AHelikaActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AHelikaActor);
	AHelikaActor::~AHelikaActor() {}
	struct Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::EnumInfo[] = {
		{ HelikaEnvironment_StaticEnum, TEXT("HelikaEnvironment"), &Z_Registration_Info_UEnum_HelikaEnvironment, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 922295950U) },
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::ScriptStructInfo[] = {
		{ FHEvents::StaticStruct, Z_Construct_UScriptStruct_FHEvents_Statics::NewStructOps, TEXT("HEvents"), &Z_Registration_Info_UScriptStruct_HEvents, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FHEvents), 1766944434U) },
		{ FHSession::StaticStruct, Z_Construct_UScriptStruct_FHSession_Statics::NewStructOps, TEXT("HSession"), &Z_Registration_Info_UScriptStruct_HSession, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FHSession), 3048710806U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AHelikaActor, AHelikaActor::StaticClass, TEXT("AHelikaActor"), &Z_Registration_Info_UClass_AHelikaActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AHelikaActor), 2157141202U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_1741119576(TEXT("/Script/Helika"),
		Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::ScriptStructInfo),
		Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_hamza_Documents_Unreal_Projects_HelikaPoject_Plugins_Helika_Source_Helika_Public_HelikaActor_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
