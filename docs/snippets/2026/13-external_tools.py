import subprocess
import sys
import os
import glob


def run_command(cmd, description):
    """外部コマンドを実行し、エラーがあれば停止"""
    print(f"\n>>> {description}...")
    try:
        subprocess.run(cmd, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"\nError during {description}")
        sys.exit(1)


def svg_to_png(out_dir, dpi):
    """SVGをPNGに変換"""
    run_command(
        f"mogrify -format png -density {dpi} {out_dir}/*.svg",
        "It may take a while...",
    )
    for f in glob.glob(os.path.join(out_dir, "*.svg")):
        os.remove(f)


def compile_video(board_name, layer, list_file, fps, width, crf):
    """PNG画像をつなぎ合わせて動画にする"""
    out_mp4 = f"{board_name}_{layer}.mp4"
    out_gif = f"{board_name}_{layer}.gif"
    filters = f"pad=ceil(iw/2)*2:ceil(ih/2)*2,scale={width}:-1:flags=lanczos"

    run_command(
        f"ffmpeg -y -f concat -safe 0 -i {list_file} -vf '{filters},format=yuv420p' -r {fps} -c:v libx264 -crf {crf} {out_mp4} >/dev/null 2>&1",
        "Compiling High-Quality Video",
    )

    # GIFに変換
    run_command(
        f"ffmpeg -y -i {out_mp4} {out_gif} >/dev/null 2>&1",
        "Converting MP4 to High-Quality GIF",
    )
