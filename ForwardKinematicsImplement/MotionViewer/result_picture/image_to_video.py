import subprocess
subprocess.call(
"./ffmpeg -r 30 -i %05d.png -c:v libx264 -qp 1 -profile:v high444 -preset fast -pix_fmt yuv420p result.mov",
shell = True
);