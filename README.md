# fasmx

An experimental Linux/X11 port of the flat assembler Windows IDE (FASMW), written in assembly. It combines a source editor with the FASM assembler and communicates directly with X11.

## An experiment in LLM-assisted porting

This project explores whether LLMs can port assembly programs from one operating system to another. The port was performed solely by Claude Code, with subsequent bugs fixed using Codex.

**This is a work in progress and far from finished.** Expect bugs, missing features, and incomplete behavior. It is an experiment, not a production-ready development tool.

## Building and running

Build with flat assembler:

```sh
fasm fasmx.asm fasmx
chmod +x fasmx
./fasmx
```

The application requires a local X11 display and support for running 32-bit Linux executables.

## Credits and license

Based on flat assembler and the FASMW IDE by Tomasz Grysztar. Original copyright notices are preserved.

See [license.txt](license.txt) for the license and redistribution terms.
