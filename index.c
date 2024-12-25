// -------- preprocessor -----------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef _win32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

// -------- terminal color code -----------
#define reset "\x1b[0m"
#define cyan "\x1b[36m"
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define red "\x1b[31m"
#define blue "\x1b[34m"

// -------- inital values regarding the subjects and days -----------
#define max_days 7
#define max_subjects 8
#define max_line_length 100

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
void prioritize_subjects(char timetable[max_days][max_line_length], int days,
                         int tomorrow_index) {
  char subjects[max_subjects][50];
  int forward_counts[max_subjects] = {0};
  int backward_counts[max_subjects] = {0};
  int difficulty[max_subjects];
  int subject_count = 0;

  for (int day_offset = 0; day_offset < 2; day_offset++) {
    int day_index = (tomorrow_index + day_offset) % days;
    char *token = strtok(timetable[day_index], ", ");
    while (token != null) {
      int found = 0;
      for (int i = 0; i < subject_count; i++) {
        if (strcmp(subjects[i], token) == 0) {
          forward_counts[i]++;
          found = 1;
          break;
        }
      }
      if (!found) {
        strncpy(subjects[subject_count], token,
                sizeof(subjects[subject_count]));
        forward_counts[subject_count] = 1;
        subject_count++;
      }
      token = strtok(null, ", ");
    }
  }

  for (int day_offset = 0; day_offset <= 2; day_offset++) {
    int day_index = (tomorrow_index - day_offset + days) % days;
    char *token = strtok(timetable[day_index], ", ");
    while (token != null) {
      for (int i = 0; i < subject_count; i++) {
        if (strcmp(subjects[i], token) == 0) {
          backward_counts[i]++;
          break;
        }
      }
      token = strtok(null, ", ");
    }
  }

  animated_text(green "\ncalculating initial priorities:\n" reset, 1);
  for (int i = 0; i < subject_count; i++) {
    printf(yellow "#%d %s: " reset, i + 1, subjects[i]);
    printf("upcoming: %d, recent: %d\n", forward_counts[i], backward_counts[i]);
  }

  printf(green
         "\nrank the difficulty of the subjects (1=easy, 10=hard):\n" reset);
  for (int i = 0; i < subject_count; i++) {
    printf(yellow "%s: " reset, subjects[i]);
    scanf("%d", &difficulty[i]);
  }

  int scores[max_subjects];
  for (int i = 0; i < subject_count; i++) {
    scores[i] = forward_counts[i] * 2 - backward_counts[i] + difficulty[i];
  }

  for (int i = 0; i < subject_count - 1; i++) {
    for (int j = i + 1; j < subject_count; j++) {
      if (scores[i] < scores[j]) {
        int temp_score = scores[i];
        scores[i] = scores[j];
        scores[j] = temp_score;

        char temp_subject[50];
        strncpy(temp_subject, subjects[i], sizeof(temp_subject));
        strncpy(subjects[i], subjects[j], sizeof(subjects[i]));
        strncpy(subjects[j], temp_subject, sizeof(subjects[j]));

        int temp_count = forward_counts[i];
        forward_counts[i] = forward_counts[j];
        forward_counts[j] = temp_count;

        temp_count = backward_counts[i];
        backward_counts[i] = backward_counts[j];
        backward_counts[j] = temp_count;

        temp_count = difficulty[i];
        difficulty[i] = difficulty[j];
        difficulty[j] = temp_count;
      }
    }
  }

  animated_text(green "\nfinal priorities with recommendations:\n" reset, 1);
  for (int i = 0; i < subject_count; i++) {
    printf(yellow "#%d %s (score = %d): " reset, i + 1, subjects[i], scores[i]);
    if (scores[i] > 15) {
      printf("highly recommended to focus on this subject. it is both upcoming "
             "and challenging.\n");
    } else if (scores[i] > 10) {
      printf("recommended to revise. important but manageable.\n");
    } else {
      printf("you seem comfortable with this. minimal effort required.\n");
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
