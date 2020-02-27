meta:
  id: player_info
  endian: le

seq:
  - id: version
    type: u8
  - id: xuid
    type: u8
  - id: name
    type: strz
    size: max_player_name_length
    encoding: UTF-8
  - id: user_id
    type: s4
  - id: guid
    type: strz
    size: signed_guid_str_len
    encoding: ascii
  - id: friends_id
    type: s4
  - id: friends_name
    type: strz
    size: max_player_name_length
    encoding: UTF-8
  - id: fakeplayer
    type: u1
  - id: hltv
    type: u1
  - id: custom_files
    type: u4
    repeat: expr
    repeat-expr: max_custom_files
  - id: files_downloaded
    type: u1
  - id: entity_id
    type: s4

instances:
  max_player_name_length:
    value: 128
  max_custom_files:
    value: 4
  signed_guid_len:
    value: 32
  signed_guid_str_len:
    value: signed_guid_len + 1
