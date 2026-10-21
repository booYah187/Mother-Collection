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
# sha1:  306fb8874533b0fd0796208faa8bed2db4ae77fb

[[extra_func]]
addr = 0x03000000
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x3611"

[[extra_func]]
addr = 0x03002EF0
mode = "arm"
note = "proposed from self-heal miss-log; bridged x4133"

[[extra_func]]
addr = 0x03005714
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1956"

[[extra_func]]
addr = 0x0801CBF2
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x0801CD0E
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x3"

[[extra_func]]
addr = 0x0801CFFE
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x08021024
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x119"

[[extra_func]]
addr = 0x0802109E
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x119"

[[extra_func]]
addr = 0x0802127C
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x119"

[[extra_func]]
addr = 0x0804A306
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x0804B784
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x080558AC
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x6"

[[extra_func]]
addr = 0x080558C2
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x080577A6
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x83"

[[extra_func]]
addr = 0x08057834
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x08057AA4
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x08057AF4
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x08057C36
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x08057C6C
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

[[extra_func]]
addr = 0x08057F3C
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

# ── JUMP-TABLE CANDIDATE: 4 consecutive thumb misses in [0x08058412, 0x080584EE] ──
# Likely the case targets of a computed-jump switch the finder
# could not size. PREFER one sized [[jump_table]] over the 4
# [[extra_func]] below: find the abs32 table base (the
# `ldr rT,[pc,#..]; add rT,index<<2; ldr/mov pc` dispatcher) and
# add `[[jump_table]] addr=<base> stride=4 count=<CMP bound> format="abs32" entries_mode="auto"`.
[[extra_func]]
addr = 0x08058412
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x3; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x0805845A
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x080584CA
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x22; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x080584EE
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x2; part of a JUMP-TABLE CANDIDATE (prefer a sized [[jump_table]])"

[[extra_func]]
addr = 0x08129B92
mode = "thumb"
note = "proposed from self-heal miss-log; bridged x1"

