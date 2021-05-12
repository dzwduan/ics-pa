#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_DEC,TK_HEX

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-",  '-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"0|([1-9][0-9]*)",TK_DEC},
  {"0[Xx][0-9A-Fa-f]+",TK_HEX},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

//用于记录token信息
typedef struct token {
  int type;
  //记录子串
  char str[32];
} Token;

//attribute__((used))的函数被标记在目标文件中，以避免链接器删除未使用的节。
//存放已经被识别出来的token信息
static Token tokens[32] __attribute__((used)) = {};
//已经被识别出的token数目
static int nr_token __attribute__((used))  = 0;
//e是输入字符串
static bool make_token(char *e) {
  //当前处理到的位置
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    //依次尝试所有规则
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        //记录到tokens
        switch (rules[i].token_type) {
          //空格不必记录
          case TK_NOTYPE:break;
          //下面的只需要记录类型
          case '+':
          case '-':
          case '*':
          case '/':
          case '(':
          case ')':
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
          //记录类型+字符串
          case TK_HEX:
          case TK_DEC:
          case TK_EQ:
            tokens[nr_token].type = rules[i].token_type;
            memset(tokens[nr_token].str,0,32);
            assert(strlen(substr_start)<32);
            memcpy(tokens[nr_token].str,substr_start);
            nr_token++;
            break;

          default: 
           break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

//======================helper functions==================

static bool check_parentheses(int p,int q){
  //出现（ +1
  //出现 ）-1
  //sum < 0 fasle
  //final sum!=0 false
  int sum = 0;
  int i=p;
  for(;i<q;i++){
    if(tokens[p].type=='(') sum++;
    if(tokens[p].type==')') sum--;
    if(sum<0) return false;
  }
  return sum==0;
}

static int getMainOpt(int p,int q){

}

//p和q指示表达式开始和结束位置
static word_t eval(int p, int q) {
  if (p > q) {
    /* Bad expression */
    printf("Bad expression\n");
    return 0;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if(tokens[p].type==TK_DEC)
      return atoi(tokens[p].str);
    if(tokens[p].type==TK_HEX){
      word_t num = 0;
      sscanf(tokens[p].str,"0x%x",&num);
      return num;
    }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    /* We should do more things here. */
    int op = getMainOpt();
    word_t val1 = eval(p, op - 1);
    word_t val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default: 
        printf("can not recognition type\n");
        break;
  }
}

word_t expr(char *e, bool *success) {
  //check make token
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */



  return 0;
}
