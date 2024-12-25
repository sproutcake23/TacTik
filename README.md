# 📚 TakTic 🕛

**A smart CLI-based study planner that helps students prioritize their subjects effectively**

```
      _---~~(~~-_.
    _{        )   )
  ,   ) -~~- ( ,-' )_
 (  `-,_..`., )-- '_,)
( ` _)  (  -~( -_ `,  }
(_-  _  ~_-~~~~`,  ,' )
  `~ -^(    __;-,((()))
        ~~~~ {_ -_(())
               `\  }
                 { }
```

</div>

## 🌟 Features

- **Smart Prioritization**: Analyzes your timetable to suggest study priorities
- **Dual Input Modes**: Choose between interactive input or file-based planning
- **Scientific Approach**: Implements spaced repetition principles
- **Intelligent Scoring**: Considers subject difficulty and frequency
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Beautiful CLI**: Features loading animations and colored output

## 🚀 Installation

### Run from EXE

1. Double click on the executable file

### Building from Source

1. Clone the repository

```bash
git clone https://github.com/sproutcake23/tactik.git
cd tactik
```

2. Compile the program

```bash
# Using GCC directly
gcc -o taktik index.c

# Or using Make
make
```

3. Run the program

```bash
./taktik
```

## 💡 Usage

### Interactive Mode

1. Choose option 1 from the main menu
2. Enter the number of days in your timetable
3. Input subjects for each day (comma-separated)
4. Rate the difficulty of each subject
5. View your prioritized study plan

### File Mode

1. Create a text file with your timetable (example: `schedule.txt`). Make sure you add 7 entries in that.

```
Math, Physics, Chemistry
Biology, English, History
Computer Science, Physics, Math
Math, Physics, Chemistry
Biology, English, History
Computer Science, Physics, Math
Math, Physics, Chemistry
```

2. Choose option 2 from the main menu
3. Enter the filename when prompted
4. Rate the difficulty of each subject
5. View your prioritized study plan

## 🛠️ Technical Details

### Algorithm

The prioritization algorithm considers:

- Upcoming subject frequency
- Recent study history
- Subject difficulty ratings
- Spaced repetition principles

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

- Project Link: [https://github.com/sproutcake23/tactik](https://github.comsproutcake23/tactik)
- Report bugs: [Issue Tracker](https://github.com/sproutcake23/tactik/issues)

---

<div align="center">
Made with ❤️ for students everywhere
</div>
