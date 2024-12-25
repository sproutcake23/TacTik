// -------- preprocessor -----------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

// -------- terminal color code -----------
#define RESET "\x1b[0m"
#define CYAN "\x1b[36m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define BLUE "\x1b[34m"

// -------- inital values regarding the subjects and days -----------
#define MAX_DAYS 7
#define MAX_SUBJECTS 8
#define MAX_LINE_LENGTH 100

// -------- function declaration -----------
void print_welcome();
void display_loading_screen();
void animated_text(const char *text, int center);
void read_interactive(char timetable[max_days][max_line_length], int *days);
void read_file_mode(char filename[], char timetable[max_days][max_line_length],
                    int *days);
void prioritize_subjects(char timetable[max_days][max_line_length], int days,
                         int tomorrow_index);

int count_occurrences(const char *line, const char *subject);
int get_terminal_width();
int get_tomorrow_index();
const char *welcome_quote();
void display_menu();
void about_us();
void exit_program();

// -------- main function -----------
int main() {
  char timetable[max_days][max_line_length];
  int days = 0;
  int choice;

  print_welcome();

  while (1) {
    display_menu();

    printf(yellow "your choice: " reset);
    scanf("%d", &choice);
    getchar();

    switch (choice) {
    case 1:
      read_interactive(timetable, &days);
      break;
    case 2: {
      char filename[50];
      printf(green "please enter the file name: " reset);
      fgets(filename, sizeof(filename), stdin);
      strtok(filename, "\n");
      read_file_mode(filename, timetable, &days);
    } break;
    case 3:
      about_us();
      break;
    case 4:
      exit_program();
      return 0;
    default:
      printf(red "invalid choice. please try again." reset "\n");
      break;
    }

    if (choice == 1 || choice == 2) {
      int tomorrow_index = get_tomorrow_index();
      if (tomorrow_index >= 0 && tomorrow_index < days) {
        prioritize_subjects(timetable, days, tomorrow_index);
      } else {
        printf(red "invalid day index. returning to the menu." reset "\n");
      }
    }
  }

  return 0;
}

// -------- finding the terminal width -- helps in centering the text
// -----------
int get_terminal_width() {

  // if it is a windows terminal
#ifdef _win32
  console_screen_buffer_info csbi;
  int columns = 80;
  if (getconsolescreenbufferinfo(getstdhandle(std_output_handle), &csbi)) {
    columns = csbi.srwindow.right - csbi.srwindow.left + 1;
  }
  return columns;
  // if it is a unix terminal
#else
  struct winsize w;
  ioctl(0, tiocgwinsz, &w);
  return w.ws_col;
#endif
}

// -------- centering the text/string that is given in the argument of the
// function -----------
void center_text(const char *text) {
  int width = get_terminal_width();
  int text_len = strlen(text);
  int padding = (width - text_len) / 2;
  for (int i = 0; i < padding; i++)
    printf(" ");
  printf("%s\n", text);
}

// -------- printing the logo -----------
void logo() {
  printf(yellow);
  center_text("      _---~~(~~-_.");
  center_text("    _{        )   )");
  center_text("  ,   ) -~~- ( ,-' )_");
  center_text(" (  `-,_..`., )-- '_,)");
  center_text("( ` _)  (  -~( -_ `,  }");
  center_text("(_-  _  ~_-~~~~`,  ,' )");
  center_text("  `~ -^(    __;-,((()))");
  center_text("        ~~~~ {_ -_(())");
  center_text("               `\  }");
  center_text("                 { }   ");
  printf(" ");
  printf(" ");
  center_text("a cai - a project made entirely with c");
  printf(reset);
}

// -------- quote generator -----------
const char *welcome_quote() {

  const char *quotes[] = {
      "success is the result of preparation, hard work, and learning from "
      "failure.",
      "an investment in knowledge always pays the best interest.",
      "each new day is an opportunity to start fresh and achieve your goals.",
      "the more you learn, the more you earn and grow.",
      "by failing to prepare, you are preparing to fail.",
      "a goal without a plan is just a wish. start planning your success "
      "today.",
      "don't let yesterday take up too much of today.",
      "the best way to start the day is to plan for a productive tomorrow.",
      "learn from yesterday, live for today, and prepare for tomorrow.",
      "study hard. know more today than you did yesterday, and strive for even "
      "more tomorrow.",
      "mistakes are proof that you are trying. learn from them and move "
      "forward.",
      "dream big, plan well, and work harder than ever to achieve your goals.",
      "success in learning comes to those who study persistently and plan "
      "wisely.",
      "every morning you have two choices: continue to sleep with your dreams "
      "or wake up and chase them.",
      "planning is bringing the future into the present, so you can take "
      "action now."};

  int num_quotes = sizeof(quotes) / sizeof(quotes[0]);

  srand(time(null));
  int random_index = rand() % num_quotes;

  return quotes[random_index];
}

// -------- welcome screen -----------
void print_welcome() {
  printf("\n ");
  printf("\n");
  display_loading_screen();
  printf("\n ");
  printf("\n");
  logo();
  printf("\n ");
  printf("\n");
  printf(cyan);

  printf(" ");
  center_text("      * welcome to tactik *     ");
  center_text("=============================================");

  printf("\n");
  center_text(
      "       tactik is a thoughtfully designed tool "
      "crafted to help students take control of their study schedules. ");
  printf("\n ");
  center_text(" this innovative cli-based program goes beyond just "
              "listing tomorrow's "
              "classes;");
  center_text("      it dynamically evaluates your upcoming timetable by "
              "analyzing your recent ");
  center_text("      study patterns, upcoming workload, and personal input "
              "about subject difficulty");
  printf("\n ");
  center_text("      the result? a customized list of prioritized "
              "subjects that ensures you're fully prepared and focused on what "
              "matters most.");

  printf("\n ");
  printf("\n");

  printf(green);
  center_text("=============================================");
  const char *quote = welcome_quote();
  center_text(quote);
  center_text("=============================================");
  printf(reset);
}

// -------- displaying the choices in the welcome screen -----------
void display_menu() {
  printf(green "\nplease choose an option:\n" reset);
  printf(" ");
  printf("1. interactive mode\n");
  printf("2. file mode\n");
  printf("3. about us\n");
  printf("4. exit program\n");
  printf(" ");
}

// -------- display the loading screen animation -----------
void display_loading_screen() {
  const char *loading_message = "loading tactik application";
  const char *spinner = "|/-\\";
  int width = get_terminal_width();
  int bar_width = width - 20;

  printf(yellow "\n");
  center_text(loading_message);
  printf("\n");

  for (int i = 0; i <= bar_width; i += 2) {
    printf("\r[");
    for (int j = 0; j < i; j++) {
      printf("=");
    }
    for (int j = i; j < bar_width; j++) {
      printf(" ");
    }
    printf("] %c", spinner[i / 2 % 4]);
    fflush(stdout);
    usleep(20000);
  }
  printf("\n" reset);
}

// -------- typing text animation -----------
void animated_text(const char *text, int center) {
  if (center) {
    int width = get_terminal_width();
    int text_len = strlen(text);
    int padding = (width - text_len) / 2;
    for (int i = 0; i < padding; i++)
      printf(" ");
  }
  while (*text) {
    putchar(*text++);
    fflush(stdout);
    usleep(50000);
  }
  printf("\n");
}

// -------- finding the tomorrow's index -----------
int get_tomorrow_index() {
  time_t t = time(null);
  struct tm tm = *localtime(&t);

  return (tm.tm_wday) % 7;
}

// --------- interactive mode code -----------
void read_interactive(char timetable[max_days][max_line_length], int *days) {
  printf(yellow "enter the number of days (max %d, min 2): " reset, max_days);
  scanf("%d", days);
  getchar();

  for (int i = 0; i < *days; i++) {
    printf(green "enter subjects for day %d (comma-separated): " reset, i + 1);
    fgets(timetable[i], max_line_length, stdin);
    strtok(timetable[i], "\n");
  }
}

// --------- file reading mode -----------
void read_file_mode(char filename[], char timetable[max_days][max_line_length],
                    int *days) {
  file *file = fopen(filename, "r");
  if (!file) {
    printf(red "error: could not open file. returning to the menu." reset);
    return;
  }

  char line[max_line_length];
  *days = 0;
  while (fgets(line, sizeof(line), file) && *days < max_days) {
    strtok(line, "\n");
    strncpy(timetable[*days], line, max_line_length - 1);
    timetable[*days][max_line_length - 1] = '\0';
    (*days)++;
  }

  fclose(file);
}

// --------- prioritize subject algorithm - (today, yesterday) is compared with
// (tomorrow and day-after-tomorrow)-----------
void prioritize_subjects(char timetable[MAX_DAYS][MAX_LINE_LENGTH], int days,
                         int tomorrow_index) {
  char subjects[MAX_SUBJECTS][50];
  int scores[MAX_SUBJECTS] = {0};
  int subject_count = 0;

  char *token = strtok(timetable[tomorrow_index], ", ");
  while (token != NULL) {
    int found = 0;
    for (int i = 0; i < subject_count; i++) {
      if (strcmp(subjects[i], token) == 0) {
        found = 1;
        scores[i]++;
        break;
      }
    }
    if (!found) {
      strncpy(subjects[subject_count], token, sizeof(subjects[subject_count]));
      scores[subject_count] = 1;
      subject_count++;
    }
    token = strtok(NULL, ", ");
  }

  int max_score = scores[0];
  for (int i = 1; i < subject_count; i++) {
    if (scores[i] > max_score)
      max_score = scores[i];
  }

  double normalized_scores[MAX_SUBJECTS];
  for (int i = 0; i < subject_count; i++) {
    normalized_scores[i] = (double)scores[i] / max_score * 100.0;
  }

  printf("\nPriority Levels for Tomorrow:\n");
  for (int i = 0; i < subject_count; i++) {
    printf(GREEN "%s: " RESET, subjects[i]);
    if (normalized_scores[i] > 75.0) {
      printf(YELLOW "High Priority. Focus on mastering concepts, and ensure "
                    "preparation is thorough.\n" RESET);
    } else if (normalized_scores[i] > 50.0) {
      printf(BLUE "Medium-High Priority. Consider spending additional time on "
                  "practice and review.\n" RESET);
    } else if (normalized_scores[i] > 25.0) {
      printf(CYAN
             "Medium-Low Priority. Quick revision will be sufficient.\n" RESET);
    } else {
      printf(RED "Easy Priority. A light review to keep concepts fresh is "
                 "recommended.\n" RESET);
    }
  }
}

// --------- about us and documentation code -----------
void about_us() {
  printf("\n");

  printf(cyan "what is tactik?\n" reset);
  printf(cyan "===========================\n\n" reset);
  printf("tactik is a thoughtfully designed tool crafted to help "
         "students take control of their study schedules. ");
  printf(
      "this innovative cli-based program goes beyond just listing tomorrow's "
      "classes; it dynamically evaluates your upcoming timetable ");
  printf("by analyzing your recent study patterns, upcoming workload, and "
         "personal input about subject difficulty. the result? ");
  printf("a customized list of prioritized subjects that ensures you're fully "
         "prepared and focused on what matters most.\n\n");
  printf("with a sleek ascii-art ui, engaging animations, and intelligent "
         "algorithms working behind the scenes, ");
  printf("tactik is more than just a tool—it's your smart study "
         "companion.\n\n");

  printf(cyan "who is it useful for?\n" reset);
  printf(cyan "=====================\n\n" reset);
  printf("tactik is perfect for:\n");
  printf("- " green "students" reset
         " who want to stay ahead in their academics and make strategic use of "
         "their time.\n");
  printf("- " green "planners" reset
         " who love organization and seek to prioritize tasks based on their "
         "importance and urgency.\n");
  printf("- " green "procrastinators" reset
         " who find it difficult to decide where to start preparing and need "
         "that extra push.\n");
  printf("- " green "teachers or mentors" reset
         ", who can use this tool to suggest better ways for their students to "
         "prepare for classes or exams.\n\n");
  printf("from high school to college, this application caters to students of "
         "all levels, helping them align their study habits with their "
         "academic goals.\n\n");

  printf(cyan "the uses of spaced repetition and smart goals\n" reset);
  printf(cyan "============================================\n\n" reset);
  printf("- " yellow "spaced repetition:" reset
         " tactik integrates the principles of spaced repetition "
         "to evaluate subject focus. ");
  printf("it ensures subjects you haven't studied in recent days are given "
         "higher priority, creating natural and effective review intervals. ");
  printf("this makes retention and long-term learning far easier.\n\n");
  printf("- " yellow "smart goals:" reset
         " the program aligns its recommendations with the principles of smart "
         "(specific, measurable, achievable, relevant, time-bound) goals. ");
  printf("by identifying clear priorities for the next day and weighing the "
         "difficulty levels, tactik helps you achieve measurable ");
  printf("and realistic progress while maintaining a focused and relevant "
         "approach to your studies.\n\n");
  printf("together, these techniques bring science-backed study strategies to "
         "your fingertips.\n\n");

  printf(cyan "about the team\n" reset);
  printf(cyan "==============\n\n" reset);
  printf("tactik was developed with passion by a team dedicated "
         "to blending technology and education. ");
  printf("driven by the desire to make learning both effective and engaging, "
         "the creators come from diverse backgrounds, ");
  printf("bringing expertise in software development, education strategies, "
         "and ux design.\n\n");
  printf("we believe that small, intentional changes in how you approach "
         "learning can have a massive impact on your academic journey. ");
  printf("our goal is to inspire students to take control of their preparation "
         "process, feel less overwhelmed, and ultimately excel in their "
         "pursuits.\n\n");
  printf("when we aren't busy improving tactik, we're exploring "
         "ways to integrate modern ai and data-driven methodologies ");
  printf("to push the boundaries of educational tools further.\n\n");
  printf("let tactik simplify your study process so you can focus on "
         "success. together, let's build a brighter and smarter tomorrow!\n\n");
}

// --------- exit the program code -----------
void exit_program() {
  display_loading_screen();
  printf(green "\nthank you for using tactik!\n" reset);
  printf("we hope it helps you manage your studies effectively. have a great "
         "day ahead!\n");
  sleep(5);
}
