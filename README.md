# simplayer
A simple media player under linux which is base on SDL2, ALSA and FFmpeg.

>There is no UI in this player, the only way that can control the player is by input command when the player running. So user can develop personalized UI to control the player.
>
>Support Audio passthough (HDMI), Bits Audio passthough works well on my receiver DENON AV1912.
>
>Support .srt(pure text) and .ass subtitles.
>
>Can directly play .cue file.
>
>Can play playlist like cue file.
>

# Dependency
ffmpeg-2.2.2, freetype-2.5.3, fribidi-0.19.6, libass-0.11.2, SDL2-2.0.3 and SDL2_ttf-2.0.12

# Build
Install all dependency first and run `mt.sh -m` to create makefile then run `mt.sh -cm` and `mt.sh -install`

# Test
`mt.sh -at/-nt` to run test.

# Play
>`simplayer file [options]`:
>
>>`--print` Just print media information
>
>>`--location` Left-top of the player window, default: 0,0
>
>>`--size Size` of player window, default: 800x450
>
>>`--mode` Display mode, defaul: 0 or 1 (full screen)
>
>>`--video` Track index of the media file
>
>>`--audio` Audio index of the media file
>
>>`--sub` Subtitle index or full path of subtitle file
>
>>`--pass` Set the option to enable the audio passthrough
>
>>`--device` Alsa device name
>
>>`--track` The range of tracks when playing cue or playlist
>
>>`--begin` The begining track when playing cue or playlist
>
>Interactive command when playing:
>
>>`stop` Stop playing
>
>>`to` Seek to the input time(millisecond)
>
>>`forward` Skip the input time(millisecond), can be negative
>
>>`time` Show the playing time
>
>>`pause` Pause
>
>>`resume` Resume
>
>>`title` Show the playing title
>
>>`next` Skip to the next track
>
>>`prev` Skip to the previous track
>
>>`jump` Skip to the input track (Number start from 1)
>
>>`status` Show playing time and playing track index
>
