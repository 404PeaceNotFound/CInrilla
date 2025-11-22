from PIL import Image
import json
import math
import os

# -------------------------------------
# CONFIGURAÇÕES
# -------------------------------------
FOLDER = "assets/frames"
OUT_FOLDER = "assets/frames"
MAX_SIZE = 8192  # limite de textura
PADDING = 0      # pode deixar 0
# -------------------------------------

os.makedirs(OUT_FOLDER, exist_ok=True)

files = sorted([f for f in os.listdir(FOLDER) if f.startswith("frame_")])
frames_paths = [os.path.join(FOLDER, f) for f in files]

# Carrega primeiro frame para pegar dimensões
first = Image.open(frames_paths[0])
w, h = first.size

# Quantas colunas cabem dentro de 8192?
max_cols = MAX_SIZE // (w + PADDING)
max_rows = MAX_SIZE // (h + PADDING)

if max_cols < 1 or max_rows < 1:
    raise Exception("Frame é maior que 8192×8192 — precisa reduzir a resolução.")

frames_per_sheet = max_cols * max_rows
total_frames = len(frames_paths)
num_sheets = math.ceil(total_frames / frames_per_sheet)

meta = {
    "frameWidth": w,
    "frameHeight": h,
    "maxTextureSize": MAX_SIZE,
    "padding": PADDING,
    "numSheets": num_sheets,
    "sheets": []
}

print("Frame:", w, "x", h)
print("Max cols:", max_cols, "Max rows:", max_rows)
print("Frames por sheet:", frames_per_sheet)
print("Sheets necessários:", num_sheets)

frame_index = 0

for sheet_id in range(num_sheets):
    # Quantos frames vão neste sheet
    start = sheet_id * frames_per_sheet
    end = min(start + frames_per_sheet, total_frames)
    sheet_frames = frames_paths[start:end]

    # sheet dimensions
    sheet_cols = min(max_cols, len(sheet_frames))
    sheet_rows = math.ceil(len(sheet_frames) / max_cols)

    sheet_w = sheet_cols * (w + PADDING) - PADDING
    sheet_h = sheet_rows * (h + PADDING) - PADDING

    print(f"Gerando sheet {sheet_id}: {len(sheet_frames)} frames, {sheet_cols}x{sheet_rows}")

    sheet_img = Image.new("RGBA", (sheet_w, sheet_h))

    sheet_info = {
        "sheetId": sheet_id,
        "file": f"sheet_{sheet_id}.png",
        "frames": []
    }

    for i, path in enumerate(sheet_frames):
        img = Image.open(path).convert("RGBA")

        col = i % max_cols
        row = i // max_cols

        x = col * (w + PADDING)
        y = row * (h + PADDING)

        sheet_img.paste(img, (x, y))

        sheet_info["frames"].append({
            "globalIndex": frame_index,
            "localIndex": i,
            "x": x,
            "y": y,
            "w": w,
            "h": h,
            "sheet": sheet_id
        })

        frame_index += 1

    # salvar imagem do sheet
    out_img_path = os.path.join(OUT_FOLDER, f"sheet_{sheet_id}.png")
    sheet_img.save(out_img_path)

    meta["sheets"].append(sheet_info)

# salvar o metadata
with open(os.path.join(OUT_FOLDER, "meta.json"), "w") as f:
    json.dump(meta, f, indent=2)

print("\nFINALIZADO.")
print("Sheets gerados em:", OUT_FOLDER)
