meta:
  id: message
  endian: le
  ks-opaque-types: true
  imports:
    - vlq_base128_le

seq:
  - id: msg_type_id
    type: vlq_base128_le
  - id: length
    type: vlq_base128_le
  - id: data
    size: length.value

types:
  string_table_update:
    doc: String Update Table parser
