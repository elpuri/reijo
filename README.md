reijo
=====

Qt based ray tracer that uses QML and javascript to describe scenes.


Video encoding requires ffmpeg and libx264(rgb). I built mine using https://ffmpeg.org/trac/ffmpeg/wiki/UbuntuCompilationGuide
When configuring libx264 and ffmpeg add --enable-dynamic to the configuration line suggested by the guide and make sure the
the linker picks the right one (there might be other versions without libx264 in the libavcodec) in your environment.
