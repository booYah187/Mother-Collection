# recomp_master_misses.toml.frag — AUTO-GENERATED proposal.
# These guest PCs were reached by runtime_dispatch with no generated
# function and were bridged through the interpreter this session.
# A HUMAN reviews these and merges the genuine ones into the binary's
# config; this file is NEVER auto-merged (PRINCIPLES.md "Never
# auto-write game.toml").
#
# Proposal shapes:
#   [[extra_func]]  — a standalone missed function entry.
#   [[jump_table]]  — flagged in a comment when a tight run of
#       consecutive same-mode misses looks like a computed-jump
#       switch's case targets. Sizing the table covers the whole
#       switch in ONE entry; prefer it over the per-case [[extra_func]].
# BIOS PCs (< 0x4000) belong in bios/gba_bios.toml; cart PCs in the
# game's game.toml.
#
# game:  MOTHER3
# code:  A3UJ
# sha1:  4ffe71ce5c5e1e8d9be5fbd09968b60787169b21

[[extra_func]]
addr = 0x00000138
mode = "arm"
note = "proposed from self-heal miss-log; bridged x263"

[[extra_func]]
addr = 0x00000300
mode = "arm"
note = "proposed from self-heal miss-log; bridged x263"

[[extra_func]]
addr = 0x0000034C
mode = "arm"
note = "proposed from self-heal miss-log; bridged x1783"

[[extra_func]]
addr = 0x00002664
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x8"

# ── JUMP-TABLE CANDIDATE: 5 consecutive thumb misses in [0x00002754, 0x000027BA] ──
# Likely the case targets of a computed-jump switch the finder
# could not size. PREFER one sized [[jump_table]] over the 5
# [[extra_func]] below: find the abs32 table base (the
# `ldr rT,[pc,#..]; add rT,index<<2; ldr/mov pc` dispatcher) and
# add `[[jump_table]] addr=<base> stride=4 count=<CMP bound> format="abs32" entries_mode="auto"`.
[[extra_func]]
addr = 0x00002754
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x2; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x00002768
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x8; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0000277A
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x8; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x000027A8
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x8; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x000027BA
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x8; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x03002EF0
mode = "arm"
note = "proposed from self-heal miss-log; bridged x1526"

[[extra_func]]
addr = 0x08000000
mode = "arm"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x080008B2
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x338"

[[extra_func]]
addr = 0x080008B6
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x338"

# ── JUMP-TABLE CANDIDATE: 3 consecutive thumb misses in [0x0805694E, 0x08056980] ──
# Likely the case targets of a computed-jump switch the finder
# could not size. PREFER one sized [[jump_table]] over the 3
# [[extra_func]] below: find the abs32 table base (the
# `ldr rT,[pc,#..]; add rT,index<<2; ldr/mov pc` dispatcher) and
# add `[[jump_table]] addr=<base> stride=4 count=<CMP bound> format="abs32" entries_mode="auto"`.
[[extra_func]]
addr = 0x0805694E
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x993; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08056954
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x993; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08056980
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x993; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x080573B8
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x270"

[[extra_func]]
addr = 0x080573E4
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x722"

# ── JUMP-TABLE CANDIDATE: 4 consecutive thumb misses in [0x08057760, 0x080577E8] ──
# Likely the case targets of a computed-jump switch the finder
# could not size. PREFER one sized [[jump_table]] over the 4
# [[extra_func]] below: find the abs32 table base (the
# `ldr rT,[pc,#..]; add rT,index<<2; ldr/mov pc` dispatcher) and
# add `[[jump_table]] addr=<base> stride=4 count=<CMP bound> format="abs32" entries_mode="auto"`.
[[extra_func]]
addr = 0x08057760
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x66; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08057788
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x142; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x080577B8
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x62; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x080577E8
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x12; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x080578B4
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x710"

[[extra_func]]
addr = 0x08057A8C
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

# ── JUMP-TABLE CANDIDATE: 4 consecutive thumb misses in [0x08057E08, 0x08057ED0] ──
# Likely the case targets of a computed-jump switch the finder
# could not size. PREFER one sized [[jump_table]] over the 4
# [[extra_func]] below: find the abs32 table base (the
# `ldr rT,[pc,#..]; add rT,index<<2; ldr/mov pc` dispatcher) and
# add `[[jump_table]] addr=<base> stride=4 count=<CMP bound> format="abs32" entries_mode="auto"`.
[[extra_func]]
addr = 0x08057E08
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x2; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08057E3C
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x6; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08057E70
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08057ED0
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0805A46A
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x195"

[[extra_func]]
addr = 0x0808F124
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x17"

[[extra_func]]
addr = 0x0808F13C
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x155"

# ── JUMP-TABLE CANDIDATE: 12 consecutive thumb misses in [0x0808F260, 0x0808F2D2] ──
# Likely the case targets of a computed-jump switch the finder
# could not size. PREFER one sized [[jump_table]] over the 12
# [[extra_func]] below: find the abs32 table base (the
# `ldr rT,[pc,#..]; add rT,index<<2; ldr/mov pc` dispatcher) and
# add `[[jump_table]] addr=<base> stride=4 count=<CMP bound> format="abs32" entries_mode="auto"`.
[[extra_func]]
addr = 0x0808F260
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F266
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F274
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x7; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F27A
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x7; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F288
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x7; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2A2
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x7; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2AA
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x7; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2B2
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x7; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2B8
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x49; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2BE
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x49; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2CC
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x49; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0808F2D2
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x49; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

