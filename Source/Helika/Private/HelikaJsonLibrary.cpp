// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaJsonLibrary.h"

typedef TSharedPtr<FJsonObject> FJsonObjectPtr;
typedef TSharedPtr<FJsonValue> FJsonValuePtr;

FHelikaJsonObject UHelikaJsonLibrary::MakeJson()
{
	FHelikaJsonObject Object;
	Object.Object = MakeShareable(new FJsonObject);
	return Object;
}

const FHelikaJsonObject& UHelikaJsonLibrary::SetJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName, const FHelikaJsonValue& Value)
{
	if (JsonObject.Object.IsValid() && Value.Value.IsValid())
	{
		JsonObject.Object->SetField(FieldName, Value.Value);
	}
	return JsonObject;
}

bool UHelikaJsonLibrary::HasJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName)
{
	if (JsonObject.Object.IsValid())
	{
		return JsonObject.Object->HasField(FieldName);
	}
	return false;
}

bool UHelikaJsonLibrary::HasJsonTypedField(const FHelikaJsonObject& JsonObject, const FString& FieldName, EJsonType Type)
{
	if (JsonObject.Object.IsValid())
	{
		if (JsonObject.Object->HasField(FieldName))
		{
			return JsonObject.Object->GetField<EJson::None>(FieldName)->Type == (EJson)Type;
		}
	}
	return false;
}

const FHelikaJsonObject& UHelikaJsonLibrary::RemoveJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName)
{
	if (JsonObject.Object.IsValid())
	{
		JsonObject.Object->RemoveField(FieldName);
	}
	return JsonObject;
}

FHelikaJsonValue UHelikaJsonLibrary::GetJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName)
{
	FHelikaJsonValue Value;
	if (JsonObject.Object.IsValid())
	{
		Value.Value = JsonObject.Object->GetField<EJson::None>(FieldName);
	}
	return Value;
}

FString UHelikaJsonLibrary::ConvertJsonObjectToString(const FHelikaJsonObject& JsonObject)
{
	FString Result;
	if (JsonObject.Object.IsValid())
	{
		const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Result);
		FJsonSerializer::Serialize(JsonObject.Object.ToSharedRef(), JsonWriter);
	}
	return Result;
}

FHelikaJsonObject UHelikaJsonLibrary::ConvertStringToJsonObject(const FString& JsonString)
{
	FHelikaJsonObject Object;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	FJsonSerializer::Deserialize(Reader, Object.Object);
	return Object;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonString(const FString& StringValue)
{
	FHelikaJsonValue Value;
	Value.Value = MakeShareable(new FJsonValueString(StringValue));
	return Value;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonInt(int IntValue)
{
	FHelikaJsonValue Value;
	Value.Value = MakeShareable(new FJsonValueNumber(IntValue));
	return Value;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonFloat(float FloatValue)
{
	FHelikaJsonValue Value;
	Value.Value = MakeShareable(new FJsonValueNumber(FloatValue));
	return Value;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonBool(bool BoolValue)
{
	FHelikaJsonValue Value;
	Value.Value = MakeShareable(new FJsonValueBoolean(BoolValue));
	return Value;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonArray(const TArray<FHelikaJsonValue>& ArrayValue)
{
	FHelikaJsonValue Value;
	TArray<FJsonValuePtr> Array;
	for (const FHelikaJsonValue& V : ArrayValue)
	{
		if (V.Value.IsValid())
		{
			Array.Add(V.Value);
		}
	}
	Value.Value = MakeShareable(new FJsonValueArray(Array));
	return Value;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonObject(const FHelikaJsonObject& ObjectValue)
{
	FHelikaJsonValue Value;
	Value.Value = MakeShareable(new FJsonValueObject(ObjectValue.Object));
	return Value;
}

FHelikaJsonValue UHelikaJsonLibrary::MakeJsonNull()
{
	FHelikaJsonValue Value;
	Value.Value = MakeShareable(new FJsonValueNull());
	return Value;
}

EJsonType UHelikaJsonLibrary::JsonType(const FHelikaJsonValue& JsonValue)
{
	if (JsonValue.Value.IsValid())
	{
		return (EJsonType)JsonValue.Value->Type;
	}
	return EJsonType::None;
}

bool UHelikaJsonLibrary::IsJsonNull(const FHelikaJsonValue& JsonValue)
{
	if (JsonValue.Value.IsValid())
	{
		return JsonValue.Value->IsNull();
	}
	return true;
}

FString UHelikaJsonLibrary::ConvertJsonValueToString(const FHelikaJsonValue& JsonValue)
{
	if (JsonValue.Value.IsValid())
	{
		return JsonValue.Value->AsString();
	}
	return FString();
}

int UHelikaJsonLibrary::ConvertJsonValueToInteger(const FHelikaJsonValue& JsonValue)
{
	if (JsonValue.Value.IsValid())
	{
		int Result = 0;
		JsonValue.Value->TryGetNumber(Result);
		return Result;
	}
	return 0;
}

float UHelikaJsonLibrary::ConvertJsonValueToFloat(const FHelikaJsonValue& JsonValue)
{
	if (JsonValue.Value.IsValid())
	{
		return JsonValue.Value->AsNumber();
	}
	return 0.0f;
}

bool UHelikaJsonLibrary::ConvertJsonValueToBool(const FHelikaJsonValue& JsonValue)
{
	if (JsonValue.Value.IsValid())
	{
		return JsonValue.Value->AsBool();
	}
	return false;
}

TArray<FHelikaJsonValue> UHelikaJsonLibrary::ConvertJsonValueToArray(const FHelikaJsonValue& JsonValue)
{
	TArray<FHelikaJsonValue> Result;

	if (JsonValue.Value.IsValid())
	{
		if (JsonValue.Value->Type == EJson::Array)
		{
			for (const auto& Val : JsonValue.Value->AsArray())
			{
				FHelikaJsonValue Tmp;
				Tmp.Value = Val;
				Result.Add(Tmp);
			}
		}
	}

	return Result;
}

FHelikaJsonObject UHelikaJsonLibrary::ConvertJsonValueToObject(const FHelikaJsonValue& JsonValue)
{
	FHelikaJsonObject Object;
	if (JsonValue.Value.IsValid())
	{
		Object.Object = JsonValue.Value->AsObject();
	}
	return Object;
}
