import os
import json
import re


def update_stock():
    # 環境変数からJSONデータを取得
    payload_json = os.environ.get("PAYLOAD_ITEMS")
    if not payload_json:
        print("No payload items found.")
        return
    items = json.loads(payload_json)

    # 日本語ページ
    for item in items:
        # 購入された商品のmdファイルを特定
        name = item.get("name")
        quantity = item.get("quantity", 0)
        sku = item.get("sku")
        sku_map = {
            "prod_***": "docs/products/test.md",
            "prod_***": "docs/products/Downpour.md",
            "prod_***": "docs/products/Downpour-mini.md",
            "prod_***": "docs/products/Downpour-in-BOSS.md",
            "prod_***": "docs/products/FactoryHeadFuzz.md",
            "prod_***": "docs/products/6500Delay.md",
        }
        file_path = sku_map.get(sku)
        if not file_path:
            print(f"Unknown SKU: {sku}. Skipping...")
            continue
        else:
            print(f"Processing SKU: {sku} -> {file_path}")

        # mdファイルを読み込んで在庫を更新
        if os.path.exists(file_path):
            with open(file_path, "r", encoding="utf-8") as f:
                content = f.read()

            # 「**在庫：n**」を正規表現で検索して更新
            match_stock = re.search(r"\*\*在庫：(\d+)\*\*", content)
            current_stock = int(match_stock.group(1))
            new_stock = max(0, current_stock - quantity)
            content = re.sub(r"\*\*在庫：\d+\*\*", f"**在庫：{new_stock}**", content)
            with open(file_path, "w", encoding="utf-8") as f:
                f.write(content)

            # 在庫が0になった場合は購入ボタンをコメントアウト
            if current_stock > 0 and new_stock == 0:
                content = re.sub(
                    r"(<stripe-buy-button.*?</stripe-buy-button>)",
                    r"<!-- \1 -->",
                    content,
                    flags=re.DOTALL,
                )
                with open(file_path, "w", encoding="utf-8") as f:
                    f.write(content)

        else:
            print(f"Error: File {file_path} not found. Skipping...")

    # 英語ページ
    for item in items:
        # 購入された商品のmdファイルを特定
        name = item.get("name")
        quantity = item.get("quantity", 0)
        sku = item.get("sku")
        sku_map = {
            "prod_***": "docs/en/products/test.md",
            "prod_***": "docs/en/products/Downpour.md",
            "prod_***": "docs/en/products/Downpour-mini.md",
            "prod_***": "docs/en/products/Downpour-in-BOSS.md",
            "prod_***": "docs/en/products/FactoryHeadFuzz.md",
            "prod_***": "docs/en/products/6500Delay.md",
        }
        file_path = sku_map.get(sku)
        if not file_path:
            print(f"Unknown SKU: {sku}. Skipping...")
            continue
        else:
            print(f"Processing SKU: {sku} -> {file_path}")

        # mdファイルを読み込んで在庫を更新
        if os.path.exists(file_path):
            with open(file_path, "r", encoding="utf-8") as f:
                content = f.read()

            # 「**Stock: n**」を正規表現で検索して更新
            match_stock = re.search(r"\*\*Stock: (\d+)\*\*", content)
            current_stock = int(match_stock.group(1))
            new_stock = max(0, current_stock - quantity)
            content = re.sub(r"\*\*Stock: \d+\*\*", f"**Stock: {new_stock}**", content)
            with open(file_path, "w", encoding="utf-8") as f:
                f.write(content)

            # 在庫が0になった場合は購入ボタンをコメントアウト
            if current_stock > 0 and new_stock == 0:
                content = re.sub(
                    r"(<stripe-buy-button.*?</stripe-buy-button>)",
                    r"<!-- \1 -->",
                    content,
                    flags=re.DOTALL,
                )
                with open(file_path, "w", encoding="utf-8") as f:
                    f.write(content)

        else:
            print(f"Error: File {file_path} not found. Skipping...")


if __name__ == "__main__":
    update_stock()