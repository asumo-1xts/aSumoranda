import sys
import os
import glob
from contextlib import redirect_stdout
from board_process import BoardProcessor
from external_tools import svg_to_png
from external_tools import compile_video


def main():
    # --- 定数 ---
    DPI = 300
    WIDTH = 1920
    CRF = 15
    FPS = 30
    categories = ["ZONE", "TRACK", "FOOTPRINT", "DRAWING"]
    print("\nkicad-logue: PCB timelapse generator")
    print("=" * 40)

    # --- 初期化 ---
    print("\n[Initialization]")
    layer = sys.argv[1] if len(sys.argv) > 1 else "F_Cu"
    input_file = sys.argv[2] if len(sys.argv) > 2 else "example.kicad_pcb"
    out_base = sys.argv[3] if len(sys.argv) > 3 else input_file
    board_name = os.path.splitext(os.path.basename(out_base))[0]
    out_dir = "./out"
    if os.path.exists(out_dir):
        for f in glob.glob(os.path.join(out_dir, "*")):
            os.remove(f)

    # --- ボードの読み込みとSVG生成 ---
    print("\n[Generating SVGs from PCB]")
    bp = BoardProcessor(input_file, out_dir, layer)
    items_dict = bp.get_all_items()
    print(f"\nBoard   : {board_name}")
    print(f"Layer   : {layer}")
    print(f"Mode    : {DPI} DPI / {WIDTH}px Width\n")

    global_idx = 0
    for cat in categories:
        items = items_dict[cat]
        if not items:
            continue
        print(f"Generating SVGs for {cat:10} ({len(items)} items)")
        for i, item in enumerate(items):
            bp.plot_current_state(f"step_{global_idx:02d}_{cat}", i)
            with redirect_stdout(open(os.devnull, "w")):
                bp.remove_item(item)
        global_idx += 1

    # --- SVGをPNGに変換 ---
    print("\n[Converting SVGs to PNGs]")
    svg_to_png(out_dir, DPI)

    # --- FFmpeg用リスト作成 ---
    print("\n[Preparing FFmpeg file list]")
    print(f"Total PNG files : {len(glob.glob(os.path.join(out_dir, '*.png')))}")
    print(f"FPS             : {FPS}")
    # PNG群はreverseして組み立て順にする
    png_files = sorted(glob.glob(os.path.join(out_dir, "*.png")), reverse=True)
    list_file = os.path.join(out_dir, "file_list.txt")
    with open(list_file, "w") as f:
        for png in png_files:
            f.write(f"file '{os.path.abspath(png)}'\nduration {1 / FPS}\n")
        # 最終フレームで2秒間停止
        f.write(f"file '{os.path.abspath(png_files[-1])}'\nduration 2.0\n")

    # --- FFmpegで動画に合成 ---
    print("\n[Compiling Video with FFmpeg]")
    compile_video(board_name, layer, list_file, FPS, WIDTH, CRF)
    print("\n" + "=" * 40)
    print("COMPLETED!")
    print(f"Output files: {board_name}_{layer}.mp4 , {board_name}_{layer}.gif")


if __name__ == "__main__":
    main()
