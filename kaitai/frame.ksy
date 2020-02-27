meta:
  id: frame
  endian: le
  imports:
    - message

seq:
  - id: frame_type
    type: u1
    enum: frame_type
  - id: tick
    type: s4
  - id: player_slot
    type: u1
  - id: body
    type:
      switch-on: frame_type
      cases:
        'frame_type::dem_signon': frame_packet
        'frame_type::dem_packet': frame_packet
        'frame_type::dem_synctick': frame_synctick
        'frame_type::dem_consolecmd': frame_consolecmd
        'frame_type::dem_usercmd': frame_usercmd
        'frame_type::dem_datatables': frame_datatables
        'frame_type::dem_stop': frame_stop
        'frame_type::dem_stringtables': frame_stringtables

enums:
  frame_type:
    1: dem_signon
    2: dem_packet
    3: dem_synctick
    4: dem_consolecmd
    5: dem_usercmd
    6: dem_datatables
    7: dem_stop
    8: dem_customdata
    9: dem_stringtables

types:
  frame_packet:
    seq:
      - id: user
        type: split
        repeat: expr
        repeat-expr: 2
      - id: seq_in
        type: s4
      - id: seq_out
        type: s4
      - id: length
        type: s4
      - id: messages
        type: messages
        size: length

    types:
      messages:
        seq:
          - id: messages
            type: message
            repeat: eos

      split:
        seq:
          - id: flags
            type: s4
          - id: view_origin
            type: vector
          - id: view_angles
            type: qangle
          - id: local_view_angles
            type: qangle
          - id: view_origin2
            type: vector
          - id: view_angles2
            type: qangle
          - id: local_view_angles2
            type: qangle

        types:
          vector:
            seq:
              - id: x
                type: f4
              - id: y
                type: f4
              - id: z
                type: f4
          qangle:
            seq:
              - id: x
                type: f4
              - id: y
                type: f4
              - id: z
                type: f4


  frame_synctick:
    doc: "sync tick"

  frame_consolecmd:
    seq:
      - id: length
        type: s4
      - id: cmd
        type: strz
        encoding: ascii
        size: length

  frame_usercmd:
    seq:
      - id: length
        type: s4
      - id: cmd
        size: length

  frame_datatables:
    seq:
      - id: length
        type: s4
      - id: data_table
        size: length

  frame_stop:
    doc: "end of demo"

  frame_stringtables:
    seq:
      - id: length
        type: s4
      - id: string_table
        type: string_tables
        size: length

    types:
      string_tables:
        seq:
          - id: num_tables
            type: u1
          - id: tables
            type: string_table
            repeat: expr
            repeat-expr: num_tables

        types:
          string_table:
            seq:
              - id: tablename
                type: strz
                encoding: ascii
              - id: num_strings
                type: s2
              - id: strings
                type: strz
                encoding: ascii
                repeat: expr
                repeat-expr: num_strings