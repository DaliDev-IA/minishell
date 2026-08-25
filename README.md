# Minishell — Unix Shell Implementation in C

A **42 School** systems-programming project that recreates a compact Unix shell in C.

The project parses user input, expands variables, handles redirections and pipelines, executes built-in and external commands, manages environment variables, and reproduces core shell behavior with careful process and signal management.

## What this project demonstrates

- Unix process creation and execution
- Pipes and inter-process communication
- File-descriptor duplication and redirection
- Command parsing and tokenization
- Environment-variable expansion
- Built-in command implementation
- Signal handling
- Error propagation and exit-status management
- Dynamic memory management in a long-running interactive program
- Team-based software development

## Implemented shell features

The project includes support for:

- interactive command input with Readline
- command history
- external command execution
- pipelines with `|`
- input and output redirections
- append redirection with `>>`
- heredoc with `<<`
- environment-variable expansion
- command-path lookup through `PATH`
- signal behavior adapted for the shell and child processes

## Built-in commands

The implementation includes the main required built-ins:

- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

## Architecture

The source tree is divided by responsibility:

- `parsing/` — tokenizer and parser
- `expansion/` — variable expansion
- `execution/` — external commands, pipelines and redirections
- `builtins/` — shell built-ins
- `environment/` — environment storage and updates
- `init/` — shell and signal initialization
- `main/` — interactive loop and lifecycle

A personal `libft` implementation is included as a support library.

## Build

```bash
make
```

This generates:

```text
minishell
```

The project links against GNU Readline.

## Usage

```bash
./minishell
```

Example commands:

```bash
minishell$ echo "$USER"
minishell$ cat file.txt | grep hello > result.txt
minishell$ export PROJECT=minishell
```

## Why it matters

Minishell is one of the projects that most directly connected my earlier C fundamentals with real operating-system behavior. It requires several subsystems to work together continuously: parsing, process creation, file descriptors, signals, environment state and memory management.

It was also a collaborative project, making architecture, interfaces and coordination between contributors part of the engineering challenge.

## Project context

This repository is part of my **42 School software-engineering journey**. The source code is intentionally public as a record of my learning progression and technical work.

---

**Mohamed Ali Chamsa**  
Software & AI Builder · Entrepreneur · 42 School