import subprocess

process = subprocess.Popen("D:/projects/auto_chess/api_server/stockfish/engine.exe",universal_newlines=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
data = process.stdin = "uci"
print(data)