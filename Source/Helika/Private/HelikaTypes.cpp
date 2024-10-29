// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaTypes.h"

#include "HelikaDefines.h"
#include "HelikaJsonLibrary.h"

void UAppDetails::Initialize(FString InPlatformId, FString InClientAppVersion, FString InServerAppVersion, FString InStoreId, FString InSourceId)
{	PlatformId = InPlatformId;
	ClientAppVersion = InClientAppVersion;
	ServerAppVersion = InServerAppVersion;
	StoreId = InStoreId;
	SourceId = InSourceId;
}

TSharedPtr<FJsonObject> UAppDetails::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField("platform_id", PlatformId);
	JsonObject->SetStringField("client_app_version", ClientAppVersion);
	JsonObject->SetStringField("server_app_version", ServerAppVersion);
	JsonObject->SetStringField("store_id", StoreId);
	JsonObject->SetStringField("source_id", SourceId);
	
	return JsonObject;
}

FString UAppDetails::ToJsonString() const
{
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(ToJson().ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return FString();
	}

	return JsonString;
}

FHelikaJsonObject UAppDetails::ToHelikaJsonObject() const
{
	FHelikaJsonObject HelikaJsonObject;
	HelikaJsonObject.Object = ToJson();
	return HelikaJsonObject;
}

void UUserDetails::Initialize(FString InUserId, FString InEmail, FString InWalletId)
{
	UserId = InUserId;
	Email = InEmail;
	WalletId = InWalletId;
}

TSharedPtr<FJsonObject> UUserDetails::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField("user_id", UserId);
	JsonObject->SetStringField("email", Email);
	JsonObject->SetStringField("wallet_id", WalletId);
	
	return JsonObject;
}

FString UUserDetails::ToJsonString() const
{
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(ToJson().ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return FString();
	}

	return JsonString;
}

FHelikaJsonObject UUserDetails::ToHelikaJsonObject() const
{
	FHelikaJsonObject HelikaJsonObject;
	HelikaJsonObject.Object = ToJson();
	return HelikaJsonObject;
}



