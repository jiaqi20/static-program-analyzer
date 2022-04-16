import sys
import subprocess
from pip import main
import pkg_resources
import argparse

print("[INFO] Checking required python packages...")

required = {'numpy', 'scipy', 'matplotlib', 'memory-profiler'}
installed = {pkg.key for pkg in pkg_resources.working_set}
missing = required - installed

if missing:
    print("[INFO] Installing missing packages: {}".format(list(missing)))
    python = sys.executable
    subprocess.check_call([python, '-m', 'pip', 'install', *missing], stdout=subprocess.DEVNULL)
else:
    print("[INFO] Requirement satisfied.")


parser = argparse.ArgumentParser()
parser.add_argument('-c', '--command', type=str, nargs=4, required=True, help="Command to run autotester")
parser.add_argument('-t', '--interval', default=0.01, help="Sampling rate of capturing memory usage")

args = parser.parse_args()
    
if args.command:
    command = list(args.command)
    result = subprocess.run(['mprof', 'run', '-T', str(args.interval), '-C'] + command, stdout=subprocess.DEVNULL)
    peak_memory_log = subprocess.run(['mprof', 'peak'], capture_output=True, text=True)
    peak_memory = ' '.join(peak_memory_log.stdout.split()[-2:])
    print("[MEM USAGE] Peak Memory Usage: {}".format(peak_memory))