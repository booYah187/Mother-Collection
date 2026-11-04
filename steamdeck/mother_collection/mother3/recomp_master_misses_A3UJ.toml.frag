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

