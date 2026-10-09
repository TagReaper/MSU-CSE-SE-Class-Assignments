#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/wait.h>

#define GAME_FILE "game_data.dat"
#define NUM_GAMES 10

//Error checking and response
int checkError(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}

// Helper Functions
int read_total() {
  int total = 0;
  int fd = checkError(open(GAME_FILE, O_RDONLY), "Failed to open game file.");
  read(fd, &total, sizeof(total));
  close(fd);
  return total;
}

void write_total(int _total) {
  int fd = checkError(open(GAME_FILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "Failed to open file.");
  write(fd, &_total, sizeof(_total));
  close(fd);
}

// Game Functions
int move_21(int _id, int _total) {
  // Player 1
  if (_id == 1) {
    return rand() % 3 + 1;
  }
  // Player 2
  return 4-(_total%4);
}

int move_100(int _id, int _total) {
  int move = 0;

  // Player 1
  if (_id == 1) {
    if (_total >= 90) {
      move = 100-_total;
    } else {
      move = ((1-(_total%11)) + 11) % 11;
    }
  } else {
    // Player 2
    move = rand() % 10 + 1;
  }

  // If number overshoots 100
  if (move > 100 - _total) {
    move = 100 - _total;
  }

  if (move == 0) {
    move = rand() % 10 + 1;
  }
  
  return move;
}

int check_result(int _game, int _recent_player, int _total) {
  int other_player = (_recent_player == 1) ? 2 : 1;

  if (_game == 0) {
    return (_total >=21) ? other_player : 0;
  }
  return (_total == 100) ? _recent_player : 0;
}

// Child Process
void child_main(int _id, pid_t _parent) {
  srand(time(NULL) + _id);

  int my_sig = 0;
  if (_id == 1) {
    my_sig = SIGUSR1;
  } else {
    my_sig = SIGUSR2;
  }

  sigset_t choose_set;
  sigset_t play_set;

  int n;
  int sig;

  sigemptyset(&choose_set);
  sigaddset(&choose_set, SIGUSR1);
  sigaddset(&choose_set, SIGUSR2);
  sigaddset(&choose_set, SIGTERM);

  sigemptyset(&play_set);
  sigaddset(&play_set, SIGUSR1);
  sigaddset(&play_set, SIGHUP);
  sigaddset(&play_set, SIGTERM);

  while (1) {
    kill(_parent, my_sig);

    if (sigwait(&choose_set, &sig) != 0) {
      exit(1);
    }
    if (sig == SIGTERM) {
      exit(0);
    }
    int game = (sig == SIGUSR1) ? 0 : 1;

    kill(_parent, my_sig);

    while (1) {
      if (sigwait(&play_set, &sig) != 0) {
        exit(1);
      }
      if (sig == SIGTERM) {
        exit(0);
      }
      if (sig == SIGHUP) {
        break;
      }

      int total = read_total();
      int move = 0;
      if (game == 0) {
        move = move_21(_id, total);
      } else {
        move = move_100(_id, total);
      }

      total += move;
      write_total(total);

      printf("Player %d: Adds %d to make the total %d.\n", _id, move, total);

      kill(_parent, my_sig);
    }
  }
}

// Main Function
int main() {
  srand(time(NULL) + getpid());

  pid_t ids[2] = {0, 0};
  int score[2] = {0, 0};
  sigset_t block_set;
  sigset_t player1_set;
  sigset_t player2_set;
  sigset_t both_set;
  int n = 0;
  int got = 0;

  sigemptyset(&block_set);
  sigaddset(&block_set, SIGUSR1);
  sigaddset(&block_set, SIGUSR2);
  sigaddset(&block_set, SIGHUP);
  sigaddset(&block_set, SIGTERM);
  if (sigprocmask(SIG_BLOCK, &block_set, NULL) < 0) {
    exit(1);
  }

  sigemptyset(&player1_set);
  sigaddset(&player1_set, SIGUSR1);

  sigemptyset(&player2_set);
  sigaddset(&player2_set, SIGUSR2);

  sigemptyset(&both_set);
  sigaddset(&both_set, SIGUSR1);
  sigaddset(&both_set, SIGUSR2);

  write_total(0);

  for (int id = 1; id <= 2; id++) {
    pid_t p = fork();

    if (p < 0) {
      exit(1);
    }
    if (p == 0) {
      child_main(id, getppid());
      exit(0);
    }
    ids[id-1] = p;
  }

  for (int x = 1; x <= NUM_GAMES; x++) {
    int have1 = 0;
    int have2 = 0;

    while (have1 == 0 || have2 == 0) {
      if (sigwait(&both_set, &got)) {
        exit(1);
      }
      if (got == SIGUSR1) {
        have1 = 1;
      }
      if (got == SIGUSR2) {
        have2 = 1;
      }
    }

    int game = rand() % 2;
    int game_sig = 0;
    if (game == 0) {
      game_sig = SIGUSR1;
    } else {
      game_sig = SIGUSR2;
    }
    write_total(0);

    printf("\nGame %d: the %d game \n", x, (game == 0) ? 21 : 100);

    kill(ids[0], game_sig);
    kill(ids[1], game_sig);

    have1 = 0;
    have2 = 0;
    while (have1 == 0 || have2 == 0) {
      if (sigwait(&both_set, &got)) {
        exit(1);
      }
      if (got == SIGUSR1) {
        have1 = 1;
      }
      if (got == SIGUSR2) {
        have2 = 1;
      }
    }

    int winner = 0;

    while (winner == 0) {
      for (int j = 1; j <= 2 && winner == 0; j++) {
        kill(ids[j - 1], SIGUSR1);

        if (sigwait(j == 1 ? &player1_set : &player2_set, &got) != 0) {
          exit(1);
        }

        winner = check_result(game, j, read_total());
        if (winner != 0) {
          kill(ids[0], SIGHUP);
          kill(ids[1], SIGHUP);
        }
      }
    }

    score[winner - 1]++;
    printf("Game %d result: Player %d wins.\nScore: P1 = %d, P2 = %d\n.", x, winner, score[0], score[1]);
  }

  kill(ids[0], SIGTERM);
  kill(ids[1], SIGTERM);
  waitpid(ids[0], NULL, 0);
  waitpid(ids[1], NULL, 0);

  char win[20];
  if (score[0] > score[1]) {
    strcpy(win, "Player 1 Wins!");
  } else if (score[1] > score[0]) {
    strcpy(win, "Player 2 Wins!");
  } else {
    strcpy(win, "It's a tie!");
  }
  printf("\nFinal score: P1 = %d, P2 %d\n Overall result: %s\n", score[0], score[1], win);

  return 0;
}