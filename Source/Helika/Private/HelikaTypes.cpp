// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaTypes.h"

#include "HelikaDefines.h"
#include "HelikaJsonLibrary.h"

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




