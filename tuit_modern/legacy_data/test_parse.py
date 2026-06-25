import re

def parse_insert(sql_line):
    # This is a very simple parser. It won't handle all edge cases but works for standard dumps.
    # It extracts the table name and the values string.
    match = re.match(r"INSERT INTO `(\w+)` [^\)]+\) VALUES\s*(.+);", sql_line, re.IGNORECASE)
    if not match:
        return None, None
    table = match.group(1)
    values_str = match.group(2)
    return table, values_str

with open("tuit_modern/legacy_data/tuit.sql", "r", encoding="utf-8", errors="ignore") as f:
    for line in f:
        if line.startswith("INSERT INTO"):
            table, vals = parse_insert(line.strip())
            if table:
                print(f"Found inserts for {table}, length of vals: {len(vals)}")
