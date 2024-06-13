// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg_ss_common.proto

#include "msg_ss_common.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace SSMessage {
class ServerFirstPackageDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<ServerFirstPackage> _instance;
} _ServerFirstPackage_default_instance_;
}  // namespace SSMessage
static void InitDefaultsscc_info_ServerFirstPackage_msg_5fss_5fcommon_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::SSMessage::_ServerFirstPackage_default_instance_;
    new (ptr) ::SSMessage::ServerFirstPackage();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::SSMessage::ServerFirstPackage::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_ServerFirstPackage_msg_5fss_5fcommon_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_ServerFirstPackage_msg_5fss_5fcommon_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_msg_5fss_5fcommon_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_msg_5fss_5fcommon_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_msg_5fss_5fcommon_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_msg_5fss_5fcommon_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::SSMessage::ServerFirstPackage, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::SSMessage::ServerFirstPackage, external_ip_),
  PROTOBUF_FIELD_OFFSET(::SSMessage::ServerFirstPackage, external_port_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::SSMessage::ServerFirstPackage)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::SSMessage::_ServerFirstPackage_default_instance_),
};

const char descriptor_table_protodef_msg_5fss_5fcommon_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\023msg_ss_common.proto\022\tSSMessage\"@\n\022Serv"
  "erFirstPackage\022\023\n\013external_ip\030\001 \001(\014\022\025\n\re"
  "xternal_port\030\002 \001(\005b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_msg_5fss_5fcommon_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_msg_5fss_5fcommon_2eproto_sccs[1] = {
  &scc_info_ServerFirstPackage_msg_5fss_5fcommon_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_msg_5fss_5fcommon_2eproto_once;
static bool descriptor_table_msg_5fss_5fcommon_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_msg_5fss_5fcommon_2eproto = {
  &descriptor_table_msg_5fss_5fcommon_2eproto_initialized, descriptor_table_protodef_msg_5fss_5fcommon_2eproto, "msg_ss_common.proto", 106,
  &descriptor_table_msg_5fss_5fcommon_2eproto_once, descriptor_table_msg_5fss_5fcommon_2eproto_sccs, descriptor_table_msg_5fss_5fcommon_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_msg_5fss_5fcommon_2eproto::offsets,
  file_level_metadata_msg_5fss_5fcommon_2eproto, 1, file_level_enum_descriptors_msg_5fss_5fcommon_2eproto, file_level_service_descriptors_msg_5fss_5fcommon_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_msg_5fss_5fcommon_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_msg_5fss_5fcommon_2eproto), true);
namespace SSMessage {

// ===================================================================

void ServerFirstPackage::InitAsDefaultInstance() {
}
class ServerFirstPackage::_Internal {
 public:
};

ServerFirstPackage::ServerFirstPackage()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SSMessage.ServerFirstPackage)
}
ServerFirstPackage::ServerFirstPackage(const ServerFirstPackage& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  external_ip_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_external_ip().empty()) {
    external_ip_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.external_ip_);
  }
  external_port_ = from.external_port_;
  // @@protoc_insertion_point(copy_constructor:SSMessage.ServerFirstPackage)
}

void ServerFirstPackage::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_ServerFirstPackage_msg_5fss_5fcommon_2eproto.base);
  external_ip_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  external_port_ = 0;
}

ServerFirstPackage::~ServerFirstPackage() {
  // @@protoc_insertion_point(destructor:SSMessage.ServerFirstPackage)
  SharedDtor();
}

void ServerFirstPackage::SharedDtor() {
  external_ip_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void ServerFirstPackage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ServerFirstPackage& ServerFirstPackage::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_ServerFirstPackage_msg_5fss_5fcommon_2eproto.base);
  return *internal_default_instance();
}


void ServerFirstPackage::Clear() {
// @@protoc_insertion_point(message_clear_start:SSMessage.ServerFirstPackage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  external_ip_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  external_port_ = 0;
  _internal_metadata_.Clear();
}

const char* ServerFirstPackage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // bytes external_ip = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_external_ip();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 external_port = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          external_port_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* ServerFirstPackage::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:SSMessage.ServerFirstPackage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes external_ip = 1;
  if (this->external_ip().size() > 0) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_external_ip(), target);
  }

  // int32 external_port = 2;
  if (this->external_port() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_external_port(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SSMessage.ServerFirstPackage)
  return target;
}

size_t ServerFirstPackage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:SSMessage.ServerFirstPackage)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes external_ip = 1;
  if (this->external_ip().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_external_ip());
  }

  // int32 external_port = 2;
  if (this->external_port() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_external_port());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ServerFirstPackage::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:SSMessage.ServerFirstPackage)
  GOOGLE_DCHECK_NE(&from, this);
  const ServerFirstPackage* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<ServerFirstPackage>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:SSMessage.ServerFirstPackage)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:SSMessage.ServerFirstPackage)
    MergeFrom(*source);
  }
}

void ServerFirstPackage::MergeFrom(const ServerFirstPackage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:SSMessage.ServerFirstPackage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.external_ip().size() > 0) {

    external_ip_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.external_ip_);
  }
  if (from.external_port() != 0) {
    _internal_set_external_port(from._internal_external_port());
  }
}

void ServerFirstPackage::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:SSMessage.ServerFirstPackage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ServerFirstPackage::CopyFrom(const ServerFirstPackage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:SSMessage.ServerFirstPackage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerFirstPackage::IsInitialized() const {
  return true;
}

void ServerFirstPackage::InternalSwap(ServerFirstPackage* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  external_ip_.Swap(&other->external_ip_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(external_port_, other->external_port_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ServerFirstPackage::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace SSMessage
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::SSMessage::ServerFirstPackage* Arena::CreateMaybeMessage< ::SSMessage::ServerFirstPackage >(Arena* arena) {
  return Arena::CreateInternal< ::SSMessage::ServerFirstPackage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
