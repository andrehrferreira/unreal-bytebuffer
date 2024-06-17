#include "ByteBuffer.h"

FString IntToBase36(int32 Value) {
    FString chars = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    FString result = TEXT("");

    if (Value == 0) 
        return TEXT("0");
       
    while (Value > 0) {
        int remainder = Value % 36;
        Value /= 36;
        result += chars[remainder];
    }
    
    result = result.Reverse();

    return result;
}

int32 Base36ToInt(const FString& Base36) {
    return FCString::Strtoi(*Base36, nullptr, 36);
}

void UBufferData::Initialize(const TMap<FString, FDynamicValue>& InputData)
{
    Data = InputData;
}

FString UBufferData::GetId(FString Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::ID)
        return Value->StringValue;
    else
        return FString();
}

FString UBufferData::GetString(FString Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::String)
        return Value->StringValue;
    else
        return FString();
}

bool UBufferData::GetBool(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::Bool)
        return Value->BoolValue;

    return false;
}

int32 UBufferData::GetInt32(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::Int32)
        return Value->IntValue;

    return 0;
}

uint32 UBufferData::GetUInt32(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::UInt32)
        return Value->UIntValue;

    return 0;
}

float UBufferData::GetFloat(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::Float)
        return Value->FloatValue;

    return 0.0;
}

uint8 UBufferData::GetByte(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::Byte)
        return Value->ByteValue;

    return 0;
}

FVector UBufferData::GetVector(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::Vector)
        return Value->VectorValue;

    return FVector(0,0,0);
}

FRotator UBufferData::GetRotator(const FString& Key) const {
    const FDynamicValue* Value = Data.Find(Key);

    if (Value && Value->ValueType == EDynamicValueType::Rotator)
        return Value->RotatorValue;

    return FRotator(0, 0, 0);
}

void UByteBuffer::EnsureCapacity(int32 RequiredBytes)
{
    int32 RequiredCapacity = Position + RequiredBytes;

    if (RequiredCapacity > Buffer.Max())
        Buffer.Reserve(RequiredCapacity);
}

UByteBuffer* UByteBuffer::CreateEmptyByteBuffer()
{
    UByteBuffer* ByteBuffer = NewObject<UByteBuffer>();
    ByteBuffer->Buffer = TArray<uint8>();
    ByteBuffer->Position = 0;
    return ByteBuffer;
}

UByteBuffer* UByteBuffer::CreateByteBuffer(const TArray<uint8>& Data = TArray<uint8>())
{
    UByteBuffer* ByteBuffer = NewObject<UByteBuffer>();
    ByteBuffer->Buffer = Data;
    ByteBuffer->Position = 0;
    return ByteBuffer;
}

UByteBuffer* UByteBuffer::CreateByteBufferFromString(const FString& Base64Data)
{
    UByteBuffer* ByteBuffer = NewObject<UByteBuffer>();

    TArray<uint8> DecodedData;
    FBase64::Decode(Base64Data, DecodedData);

    ByteBuffer->Buffer = DecodedData;
    ByteBuffer->Position = 0;

    return ByteBuffer;
}

FString UByteBuffer::GetId()
{
    int32 IdValue = GetInt32();
    FString value = IntToBase36(IdValue);
        
    return value;
}

UByteBuffer* UByteBuffer::PutId(const FString& Id)
{
    int32 IdValue = Base36ToInt(Id);
    return PutInt32(IdValue);
}

UByteBuffer* UByteBuffer::PutInt32(int32 Value)
{
    EnsureCapacity(4);

    Buffer.Add(Value & 0xFF);              
    Buffer.Add((Value >> 8) & 0xFF);        
    Buffer.Add((Value >> 16) & 0xFF);       
    Buffer.Add((Value >> 24) & 0xFF);       

    Position += 4;

    return this;
}

UByteBuffer* UByteBuffer::PutUInt32(uint32 Value)
{
    EnsureCapacity(4);

    Buffer.Add(Value & 0xFF);
    Buffer.Add((Value >> 8) & 0xFF);
    Buffer.Add((Value >> 16) & 0xFF);
    Buffer.Add((Value >> 24) & 0xFF);

    Position += 4;

    return this;
}

int32 UByteBuffer::GetInt32()
{
    if (Buffer.Num() >= Position + 4) {
        int32 result = 0;

        result |= static_cast<int32>(Buffer[Position]) << 0;
        result |= static_cast<int32>(Buffer[Position + 1]) << 8;
        result |= static_cast<int32>(Buffer[Position + 2]) << 16;
        result |= static_cast<int32>(Buffer[Position + 3]) << 24;

        Position += 4;

        return result;
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Error packet %d"), Packet);
        return 0;
    }
}

uint32 UByteBuffer::GetUInt32()
{
    if (Position + 4 > Buffer.Num()) {
        UE_LOG(LogTemp, Error, TEXT("Attempted to read beyond buffer bounds: Position=%d, BufferSize=%d"), Position, Buffer.Num());
        return 0; 
    }

    uint32 result = 0;

    result |= static_cast<uint32>(Buffer[Position]) << 0;
    result |= static_cast<uint32>(Buffer[Position + 1]) << 8;
    result |= static_cast<uint32>(Buffer[Position + 2]) << 16;
    result |= static_cast<uint32>(Buffer[Position + 3]) << 24;

    Position += 4;

    return result;
}

UByteBuffer* UByteBuffer::PutByte(uint8 Value)
{
    EnsureCapacity(1);
    Buffer.Add(Value);
    Position += 1;
    return this;
}

uint8 UByteBuffer::GetByte()
{
    if (Position >= Buffer.Num()) {
        UE_LOG(LogTemp, Error, TEXT("Attempted to read beyond buffer bounds: Position=%d, BufferSize=%d"), Position, Buffer.Num());
        return 0; 
    }

    uint8 Value = Buffer[Position];
    Position += 1;
    return Value;
}

UByteBuffer* UByteBuffer::PutString(const FString& Value)
{
    FTCHARToUTF8 Convert(*Value); 
    int32 Length = Convert.Length();
    EnsureCapacity(4 + Length); 
    PutInt32(Length);

    for (int32 i = 0; i < Length; ++i)    
        PutByte((uint8)Convert.Get()[i]);

    return this;
}

FString UByteBuffer::GetString()
{
    int32 Length = GetInt32();

    if (Length > 0 && Position + Length <= Buffer.Num())
    {
        TArray<uint8> StringBytes;
        StringBytes.Append(&Buffer[Position], Length);
        
        Position += Length;

        FString Result = FString(FUTF8ToTCHAR(reinterpret_cast<const ANSICHAR*>(StringBytes.GetData()), StringBytes.Num()));

        return Result;
    }

    return FString();
}

UByteBuffer* UByteBuffer::PutFloat(float Value) {
    EnsureCapacity(sizeof(float));
    FMemory::Memcpy(Buffer.GetData() + Position, &Value, sizeof(float));
    Position += sizeof(float);
    return this;
}

float UByteBuffer::GetFloat() {
    if (Position + sizeof(float) > Buffer.Num()) {
        UE_LOG(LogTemp, Error, TEXT("Attempted to read beyond buffer bounds: Position=%d, BufferSize=%d"), Position, Buffer.Num());
        return 0.0f; 
    }

    if (Position + sizeof(float) > Buffer.Num()) 
        return 0.0f;
    
    float Value;
    FMemory::Memcpy(&Value, Buffer.GetData() + Position, sizeof(float));
    Position += sizeof(float);
    return Value;
}

UByteBuffer* UByteBuffer::PutBool(bool Value)
{
    EnsureCapacity(1);
    uint8 ByteValue = Value ? 1 : 0;
    Buffer.Add(ByteValue);
    Position += 1;
    return this;
}

bool UByteBuffer::GetBool()
{
    if (Position + 1 > Buffer.Num())
        return false;
    
    bool Value = Buffer[Position] != 0;
    Position += 1;
    return Value;
}

UByteBuffer* UByteBuffer::PutVector(const FVector& Value)
{
    PutFloat(Value.X);
    PutFloat(Value.Y);
    PutFloat(Value.Z);
    return this;
}

FVector UByteBuffer::GetVector()
{
    FVector Value;
    Value.X = GetFloat();
    Value.Y = GetFloat();
    Value.Z = GetFloat();
    return Value;
}

UByteBuffer* UByteBuffer::PutRotator(const FRotator& Value)
{
    PutFloat(Value.Pitch);
    PutFloat(Value.Yaw);
    PutFloat(Value.Roll);
    return this;
}

FRotator UByteBuffer::GetRotator()
{
    FRotator Value;
    Value.Pitch = GetFloat();
    Value.Yaw = GetFloat();
    Value.Roll = GetFloat();
    return Value;
}

bool UByteBuffer::ReadDataFromBuffer(const TMap<FString, FString>& DataSequence, UBufferData*& OutValues, uint8 PacketID)
{
    TMap<FString, FDynamicValue> Values;
    Values.Empty();
    Packet = PacketID;

    for (const TPair<FString, FString>& Elem : DataSequence)
    {
        FDynamicValue NewValue;

        if (Elem.Value == FString("id"))
        {
            NewValue.ValueType = EDynamicValueType::ID;
            NewValue.StringValue = GetId();
        }
        else if (Elem.Value == FString("int32") || Elem.Value == FString("int"))
        {
            NewValue.ValueType = EDynamicValueType::Int32;
            NewValue.IntValue = GetInt32();
        }
        else if (Elem.Value == FString("uint32") || Elem.Value == FString("uint"))
        {
            NewValue.ValueType = EDynamicValueType::UInt32;
            NewValue.IntValue = GetUInt32();
        }
        else if (Elem.Value == FString("float"))
        {
            NewValue.ValueType = EDynamicValueType::Float;
            NewValue.FloatValue = GetFloat();
        }
        else if (Elem.Value == FString("string") || Elem.Value == FString("str"))
        {
            NewValue.ValueType = EDynamicValueType::String;
            NewValue.StringValue = GetString();
        }
        else if (Elem.Value == FString("byte"))
        {
            NewValue.ValueType = EDynamicValueType::Byte;
            NewValue.ByteValue = GetByte();
        }
        else if (Elem.Value == FString("bool") || Elem.Value == FString("boolean"))
        {
            NewValue.ValueType = EDynamicValueType::Bool;
            NewValue.BoolValue = GetBool();
        }
        else if (Elem.Value == FString("vector"))
        {
            NewValue.ValueType = EDynamicValueType::Vector;
            NewValue.VectorValue = GetVector();
        }
        else if (Elem.Value == FString("rotator"))
        {
            NewValue.ValueType = EDynamicValueType::Rotator;
            NewValue.RotatorValue = GetRotator();
        }

        Values.Add(Elem.Key, NewValue);
    }

    OutValues = NewObject<UBufferData>();
    OutValues->Initialize(Values);

    return true;
}

void UByteBuffer::WriteDataToBuffer(const TMap<FString, FString>& DataSequence, const TArray<FDynamicValue>& Values)
{
    int32 ValueIndex = 0; 

    for (const TPair<FString, FString>& Elem : DataSequence)
    {
        if (ValueIndex >= Values.Num())        
            break;
        
        const FDynamicValue& CurrentValue = Values[ValueIndex++];

        if (Elem.Value.Equals("id"))
            PutId(CurrentValue.StringValue);
        else if (Elem.Value.Equals("int32") || Elem.Value.Equals("int"))
            PutInt32(CurrentValue.IntValue);
        else if (Elem.Value.Equals("uint32") || Elem.Value.Equals("uint"))
            PutInt32(CurrentValue.UIntValue);
        else if (Elem.Value.Equals("float"))
            PutFloat(CurrentValue.FloatValue);
        else if (Elem.Value.Equals("string") || Elem.Value.Equals("str"))
            PutString(CurrentValue.StringValue);
        else if (Elem.Value.Equals("byte"))
            PutByte(CurrentValue.ByteValue);
        else if (Elem.Value.Equals("bool") || Elem.Value.Equals("boolean"))
            PutBool(CurrentValue.BoolValue);
        else if (Elem.Value.Equals("vector"))
            PutVector(CurrentValue.VectorValue);
        else if (Elem.Value.Equals("rotator"))
            PutRotator(CurrentValue.RotatorValue);
    }
}

FString UByteBuffer::ToString() const
{
    return FBase64::Encode(Buffer);
}

TArray<uint8>& UByteBuffer::GetBuffer() {
    return Buffer;
}

int32 UByteBuffer::Length() {
    return Buffer.Num();
}

void UByteBuffer::AppendBuffer(UByteBuffer* OtherBuffer)
{
    Buffer.Reserve(Buffer.Num() + OtherBuffer->Buffer.Num());
    Buffer.Append(OtherBuffer->Buffer);
    Position = Buffer.Num();
}

TArray<UByteBuffer*> UByteBuffer::SplitPackets(UByteBuffer* CombinedBuffer) {
    TArray<UByteBuffer*> Packets;
    const TArray<uint8>& BufferData = CombinedBuffer->GetBuffer();
    int32 StartPosition = 1; 
    int32 BufferSize = BufferData.Num();

    for (int32 i = 1; i <= BufferSize - 4; ++i) {
        if (
            BufferData[i] == 0xFE && 
            BufferData[i + 1] == 0xFE && 
            BufferData[i + 2] == 0xFE && 
            BufferData[i + 3] == 0xFE
        ) {
            if (i > StartPosition) {
                TArray<uint8> PacketData;
                PacketData.Append(&BufferData[StartPosition], i - StartPosition);
                UByteBuffer* PacketBuffer = UByteBuffer::CreateByteBuffer(PacketData);
                Packets.Add(PacketBuffer);
            }

            StartPosition = i + 4;
            i += 3;
        }
    }

    if (StartPosition < BufferSize) {
        TArray<uint8> PacketData;
        PacketData.Append(&BufferData[StartPosition], BufferSize - StartPosition);
        UByteBuffer* PacketBuffer = UByteBuffer::CreateByteBuffer(PacketData);
        Packets.Add(PacketBuffer);
    }

    return Packets;
}

FString UByteBuffer::ByteArrayToHexString(const TArray<uint8>& ByteArray)
{
    FString HexString;

    for (uint8 Byte : ByteArray)
        HexString += FString::Printf(TEXT("%02x"), Byte);

    return HexString;
}

FString UByteBuffer::ByteArrayToBinaryString(const TArray<uint8>& ByteArray)
{
    FString IntString;

    for (int32 i = 0; i < ByteArray.Num(); ++i)
    {
        IntString += FString::Printf(TEXT("%d"), ByteArray[i]);
        if (i < ByteArray.Num() - 1)
        {
            IntString += TEXT(", ");
        }
    }

    return IntString;
}