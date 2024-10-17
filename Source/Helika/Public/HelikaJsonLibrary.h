// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelikaJsonLibrary.generated.h"

UENUM(BlueprintType)
enum class EJsonType : uint8
{
	None,
	Null,
	String,
	Number,
	Boolean,
	Array,
	Object
};

USTRUCT(BlueprintType, meta = (HasNativeMake = "Helika.HelikaJsonLibrary.MakeJson"))
struct FHelikaJsonObject
{
	GENERATED_USTRUCT_BODY()

	TSharedPtr<class FJsonObject> Object;
};

USTRUCT(BlueprintType)
struct FHelikaJsonValue
{
	GENERATED_USTRUCT_BODY()

	TSharedPtr<class FJsonValue> Value;
};

/**
 * 
 */
UCLASS()
class HELIKA_API UHelikaJsonLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/// Creates a json object
	/// 
	/// @return Json Object
	UFUNCTION(BlueprintPure, Category = "Helika|Json", meta = (NativeMakeFunc))
	static FHelikaJsonObject MakeJson();

	/// Set the value of the field with specified name
	/// 
	/// @param JsonObject Json Object to store data to
	/// @param FieldName Name of the field to set
	/// @param Value The json value to set
	/// @return Json Object
	UFUNCTION(BlueprintCallable, Category = "Helika|Json")
	static const FHelikaJsonObject& SetJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName, const FHelikaJsonValue& Value);

	/// Checks whether a field with given field name exist in the json object
	/// 
	/// @param JsonObject Json Object containing field
	/// @param FieldName The name of the field to check
	/// @return true if json object contains that field
	UFUNCTION(BlueprintPure, Category = "Helika|Json")
	static bool HasJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName);

	/// Checks whether a field with given field name and type exist in the json object
	/// 
	/// @param JsonObject Json Object containing field
	/// @param FieldName The name of the field to check
	/// @param Type Type of the field
	/// @return true if field of type exist in the json object
	UFUNCTION(BlueprintPure, Category = "Helika|Json")
	static bool HasJsonTypedField(const FHelikaJsonObject& JsonObject, const FString& FieldName, EJsonType Type);

	/// Removes the specified field from the json object
	/// 
	/// @param JsonObject Json Object containing field
	/// @param FieldName The name of the field to remove
	/// @return Modified Json object
	UFUNCTION(BlueprintCallable, Category = "Helika|Json")
	static const FHelikaJsonObject& RemoveJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName);

	/// Get Json value from Json object
	/// 
	/// @param JsonObject source json object
	/// @param FieldName field to get
	/// @return Json value of the json object
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToHelikaJsonValue (HelikaJsonObject)", CompactNodeTitle = "ToValue", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Convert")
	static FHelikaJsonValue GetJsonField(const FHelikaJsonObject& JsonObject, const FString& FieldName);
	
	/// Converts Json object to string
	/// 
	/// @param JsonObject Json object to convert
	/// @return stringify json
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (HelikaJsonObject)", CompactNodeTitle = "ToString", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Convert")
	static FString ConvertJsonObjectToString(const FHelikaJsonObject& JsonObject);

	/// Converts a json string back to Json object
	/// 
	/// @param JsonString string to convert
	/// @return the json object
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToHelikaJsonObject (String)", CompactNodeTitle = "ToJson"), Category = "Helika|Json|Convert")
	static FHelikaJsonObject ConvertStringToJsonObject(const FString& JsonString);

	///	Create a json value string
	/// 
	/// @param StringValue value to set the string to
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta=(NativeMakeFunc))
	static FHelikaJsonValue MakeJsonString(const FString& StringValue);

	/// Create a json value int
	/// 
	/// @param IntValue value to set the int to
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta = (NativeMakeFunc))
	static FHelikaJsonValue MakeJsonInt(int IntValue);

	/// Create a json value float
	/// 
	/// @param FloatValue value to set the float to
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta = (NativeMakeFunc))
	static FHelikaJsonValue MakeJsonFloat(float FloatValue);

	/// Create a json value bool
	/// 
	/// @param BoolValue value to set the bool to
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta = (NativeMakeFunc))
	static FHelikaJsonValue MakeJsonBool(bool BoolValue);

	/// Create a json value array
	/// 
	/// @param ArrayValue value to set the array to
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta = (NativeMakeFunc))
	static FHelikaJsonValue MakeJsonArray(const TArray<FHelikaJsonValue>& ArrayValue);

	/// Create a json value object
	/// 
	/// @param ObjectValue value to set the object to
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta = (NativeMakeFunc))
	static FHelikaJsonValue MakeJsonObject(const FHelikaJsonObject& ObjectValue);

	/// Create a json null value
	/// 
	/// @return json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Make", meta = (NativeMakeFunc))
	static FHelikaJsonValue MakeJsonNull();

	/// Return type of json value
	/// 
	/// @param JsonValue json value to check
	/// @return type of json value
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Value")
	static EJsonType JsonType(const FHelikaJsonValue& JsonValue);

	/// Return true if the json value is null, false otherwise
	/// 
	/// @param JsonValue json value to check
	/// @return true if value is null
	UFUNCTION(BlueprintPure, Category = "Helika|Json|Value")
	static bool IsJsonNull(const FHelikaJsonValue& JsonValue);

	/// Converts a json value into a string
	/// 
	/// @param JsonValue json value to convert
	/// @return string value
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (JsonValue)", CompactNodeTitle = "->", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Value")
	static FString ConvertJsonValueToString(const FHelikaJsonValue& JsonValue);

	/// Converts an json value into an int
	/// 
	/// @param JsonValue json value to convert
	/// @return int value
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToInteger (JsonValue)", CompactNodeTitle = "->", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Value")
	static int ConvertJsonValueToInteger(const FHelikaJsonValue& JsonValue);

	/// Converts a json value into a float
	/// 
	/// @param JsonValue json value to convert
	/// @return float value
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFloat (JsonValue)", CompactNodeTitle = "->", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Value")
	static float ConvertJsonValueToFloat(const FHelikaJsonValue& JsonValue);

	/// Converts an json value into an bool
	/// 
	/// @param JsonValue json value to convert
	/// @return bool value
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToBool (JsonValue)", CompactNodeTitle = "->", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Value")
	static bool ConvertJsonValueToBool(const FHelikaJsonValue& JsonValue);

	/// Converts a json value into an array
	/// 
	/// @param JsonValue json value to convert
	/// @return array value
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToArray (JsonValue)", CompactNodeTitle = "->", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Value")
	static TArray<FHelikaJsonValue> ConvertJsonValueToArray(const FHelikaJsonValue& JsonValue);

	/// Converts an json value into an object
	/// 
	/// @param JsonValue json value to convert
	/// @return object value
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToJsonObject (JsonValue)", CompactNodeTitle = "->", BlueprintAutocast, NativeBreakFunc), Category = "Helika|Json|Value")
	static FHelikaJsonObject ConvertJsonValueToObject(const FHelikaJsonValue& JsonValue);	
};
