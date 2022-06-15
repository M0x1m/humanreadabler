#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <poll.h>

#define KILO 1 << 0
#define MEGA 1 << 1
#define GIGA 1 << 2
#define SI 1 << 7

char SHORTS = (KILO | MEGA | GIGA);

#define streq(s1, s2) strcmp(s1, s2) == 0
#define isdigit(c) (c > 47 && c < 58)
#define isSI ((SHORTS & SI) != 0)

void usage(FILE* stream, const char* arg0){
  fprintf(stream, "Usage: <cmd> | %s -[kmg] [args]\n", arg0);
  fprintf(stream, "  Arguments:\n");
  fprintf(stream, "    --kilo | -k   Disables kilos translates in strings\n");
  fprintf(stream, "    --mega | -m   Disables megas translates in strings\n");
  fprintf(stream, "    --giga | -g   Disables gigas translates in strings\n");
  fprintf(stream, "    --si          Uses SI(10^X) insted of 2^X\n");
  fprintf(stream, "    --help | -h   Prints this message in stdout.\n");
}

static char* hnums(unsigned long num){
  static char buf[23];
  if(num >= (isSI ? 1000 : 1 << 10) && (num <= (isSI ? 1000000 : 1 << 20) || (SHORTS & (MEGA | GIGA)) != 0) && (SHORTS & KILO) != 0){
    sprintf(buf, "%ldK", num/(isSI ? 1000 : 1 << 10));
  }else if(num >= (isSI ? 1000000 : 1 << 20) && (num <= (isSI ? 1000000000 : 1 << 30) || (SHORTS & (GIGA)) != 0) && (SHORTS & MEGA) != 0){
    sprintf(buf, "%ldM", num/(isSI ? 1000000 : 1 << 20));
  }else if(num >= (isSI ? 1000000000 : 1 << 30)/* && num <= 1LLU << 40LLU*/ && (SHORTS & GIGA) != 0){
    sprintf(buf, "%ldG", num/(isSI ? 1000000000 : 1 << 30));
  }else{
    sprintf(buf, "%ld", num);
  }
  return buf;
}

void* transl_nums(char* txt){
  char* txt2 = txt;
  char* txte = txt+strlen(txt);
  long len = 1;
  char* ret = malloc(1);
  char* rret = ret;
  while(txt2 < txte){
    char d;
    for(; (*txt2 != 0) && !(d = isdigit(*txt2)); ++txt2);
    len += txt2 - txt;
    {
      long off = ret - rret;
      rret = realloc(rret, len);
      ret = rret + off;
    }
    memcpy(ret, txt, txt2 - txt);
    rret[len-1] = 0;
    ret += txt2 - txt;
    txt = txt2;
    if(d){
      for(; isdigit(*txt2); ++txt2);
      char num[(txt2 - txt) + 1];
      num[txt2 - txt] = 0;
      memcpy(num, txt, txt2 - txt);
      const char* n = hnums(atol(num));
      len += strlen(n);
      {
        long off = ret - rret;
        rret = realloc(rret, len);
        ret = rret + off;
      }
      memcpy(ret, n, strlen(n));
      rret[len-1] = 0;
      ret += strlen(n);
      txt = txt2;
    }
  }
  return rret;
}

int parsearg(const char * arg, const char* fmt, char* target){
  if(*arg != '-') return -1;
  if(*(arg+1) == '-') return -1;
  ++arg;
  if(strlen(arg) > strlen(fmt)) return -1;
  for(int i = 0; i < strlen(arg); ++i){
    for(int j = 0; j < strlen(fmt); ++j){
      if(arg[i] == fmt[j]){
        *target &= ~(1 << j);
      }
    }
  }
  return 0;
}

int main(int argc, char** argv){
  for (int i = 1; i < argc; ++i){
    if (streq(argv[i], "-h") || streq(argv[i], "--help")){
      usage(stdout, argv[0]);
      exit(0);
    }else if (streq(argv[i], "--kilo")){
      SHORTS &= ~KILO;
    }else if (streq(argv[i], "--mega")){
      SHORTS &= ~MEGA;
    }else if (streq(argv[i], "--giga")){
      SHORTS &= ~GIGA;
    }else if (streq(argv[i], "--si")){
      SHORTS |= SI;
    }else{
      if (parsearg(argv[i], "kmg", &SHORTS) == 0) continue;
      usage(stderr, argv[0]);
      exit(1);
    }
  }  
  struct pollfd fd = {
    .fd = 0,
    .events = POLLIN | POLLRDHUP
  };
  while(1){
    poll(&fd, 1, -1);
    if (((fd.revents & POLLIN) == 0) && ((fd.revents & (POLLERR | POLLHUP | POLLRDHUP)) != 0)){
      break;
    }
    int cnt = 0;
    ioctl(0, FIONREAD, &cnt);
    if (cnt == 0) break;
    char buf[cnt+1];
    buf[cnt] = 0;
    fread(buf, cnt, 1, stdin);
    void* s;
    printf(s = transl_nums(buf));
    fflush(stdout);
    free(s);
  }
  return 0;
}
