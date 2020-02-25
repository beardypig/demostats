meta:
  id: hl2demo
  application: Source Engine
  file-extension: dem
  endian: le
  imports:
    - header
    - frame

seq:
  - id: header
    type: header
  - id: frames
    type: frame
    repeat: eos
