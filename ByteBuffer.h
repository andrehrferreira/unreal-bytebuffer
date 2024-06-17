#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/Base64.h"
#include "ByteBuffer.generated.h"

UENUM(BlueprintType)
enum class EDynamicValueType : uint8
{
	ID,
	Int32,
	UInt32,
	Float,
	Bool,
	String,
	Byte,
	Vector,
	Rotator,
	None
};

USTRUCT(BlueprintType)
struct FDynamicValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	EDynamicValueType ValueType = EDynamicValueType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	int32 IntValue;

	uint32 UIntValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	float FloatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	bool BoolValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	FString StringValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	uint8 ByteValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	FVector VectorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ByteBuffer")
	FRotator RotatorValue;
};

UCLASS(BlueprintType)
class CLIENT_API UBufferData : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const TMap<FString, FDynamicValue>& InputData);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FString GetId(FString Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FString GetString(FString Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	bool GetBool(const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	int32 GetInt32(const FString& Key) const;

	uint32 GetUInt32(const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	float GetFloat(const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	uint8 GetByte(const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FVector GetVector(const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FRotator GetRotator(const FString& Key) const;

private:
	TMap<FString, FDynamicValue> Data;
};

UCLASS(BlueprintType)
class CLIENT_API UByteBuffer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ByteBuffer")
	static UByteBuffer* CreateEmptyByteBuffer();

	UFUNCTION(BlueprintPure, Category = "ByteBuffer")
	static UByteBuffer* CreateByteBuffer(const TArray<uint8>& Data);
	
	UFUNCTION(BlueprintPure, Category = "ByteBuffer")
	static UByteBuffer* CreateByteBufferFromString(const FString& Base64Data);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	static FString ByteArrayToHexString(const TArray<uint8>& ByteArray);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	static FString ByteArrayToBinaryString(const TArray<uint8>& ByteArray);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutId(const FString& Id);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FString GetId();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutInt32(int32 Value);

	UByteBuffer* PutUInt32(uint32 Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	int32 GetInt32();

	uint32 GetUInt32();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutByte(uint8 Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	uint8 GetByte();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutString(const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FString GetString();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutFloat(float Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	float GetFloat();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutBool(bool Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	bool GetBool();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutVector(const FVector& Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FVector GetVector();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	UByteBuffer* PutRotator(const FRotator& Value);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FRotator GetRotator();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	bool ReadDataFromBuffer(const TMap<FString, FString>& DataSequence, UBufferData*& OutValues, uint8 PacketID);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	void WriteDataToBuffer(const TMap<FString, FString>& DataSequence, const TArray<FDynamicValue>& Values);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	FString ToString() const;

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	TArray<uint8>& GetBuffer();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	int32 Length();

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	void AppendBuffer(UByteBuffer* OtherBuffer);

	UFUNCTION(BlueprintCallable, Category = "ByteBuffer")
	TArray<UByteBuffer*> SplitPackets(UByteBuffer* CombinedBuffer);

private:
	TArray<uint8> Buffer;
	int32 Position = 0;
	uint8 Packet = 0;

	void EnsureCapacity(int32 RequiredBytes);
};
