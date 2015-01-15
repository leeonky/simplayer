# simplayer
A simple media player under linux which is base on SDL2, ALSA and FFmpeg.

There is no UI in this player, the only way that can control the player is by input command when the player running. So it's prossbile that user can develop personalized UI to control the player.

Bits Audio passthough works well on my receiver DENON AV1912.

Only .srt(pure text) and .ass.

simplayer can directly play .cue file.

simplayer can play playlist like cue file.

# Dependency
ffmpeg-2.2.2, freetype-2.5.3, fribidi-0.19.6, libass-0.11.2, SDL2-2.0.3 and SDL2_ttf-2.0.12

# Build
run `mt.sh -m` to create makefile first
run `mt.sh -cm` and `mt.sh -install`

# Test
`mt.sh -at/-nt` to run test.
