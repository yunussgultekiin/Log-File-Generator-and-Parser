# c-leveled-log-writer-parser

A small C command-line tool that appends timestamped, leveled entries to a daily log file and filters an existing log file by level.

Built in my first year of Computer Engineering as practice in modular C, file I/O, time formatting and command-line argument handling.

## What it does

- Writes a message with a level (`INFO`, `WARNING`, `ERROR`) to `logs/app_YYYYMMDD.log`, creating the `logs/` directory on first use.
- Appends to the same file for the rest of the day, so each day gets its own log.
- Prefixes every entry with a local timestamp: `[YYYY-MM-DD HH:MM:SS] [LEVEL] message`.
- Reads a log file in this format and prints only the entries of the requested level, with no limit on line length.

## Build

Requires a C11 compiler (`gcc` or `clang`) and `make` on Linux or macOS. On Windows, use WSL.

```bash
git clone https://github.com/yunussgultekiin/c-leveled-log-writer-parser.git
cd c-leveled-log-writer-parser
make
```

This produces the `logtool` executable. `make clean` removes build output and `make rebuild` does both.

## Usage

Write entries:

```bash
./logtool --log "Application started" --level INFO
./logtool --log "Disk usage at 91%" --level WARNING
./logtool --log "Database connection failed" --level ERROR
```

`logs/app_20261023.log` now contains:

```
[2026-10-23 20:31:35] [INFO] Application started
[2026-10-23 20:31:35] [WARNING] Disk usage at 91%
[2026-10-23 20:31:35] [ERROR] Database connection failed
```

Filter a file by level:

```bash
./logtool --parse logs/app_20261023.log --level ERROR
```

```
[2026-10-23 20:31:35] [ERROR] Database connection failed
```

Only the level tag after the timestamp is matched, so a message that mentions `[ERROR]` inside an `INFO` entry is not reported as an error.

`./logtool --help` prints the usage text. Invalid arguments, unknown levels and file errors print a message to stderr and exit with status 1.

## Project structure

```
├── main.c        argument handling and command dispatch
├── common.c/h    log levels, level name lookup and time formatting
├── logger.c/h    creating the log directory and appending entries
├── parser.c/h    reading a log file and filtering entries by level
└── Makefile
```

## License

MIT. See [LICENSE](LICENSE).
