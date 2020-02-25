meta:
  id: message
  endian: le
  imports:
    - vlq_base128_le

seq:
  - id: msg_type_id
    type: vlq_base128_le
  - id: length
    type: vlq_base128_le
  - id: data
    size: length.value
