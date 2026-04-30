
NAME = webserv

SRCS = srcs/
	   

OBJS = $(SRCS:.cpp=.o)

INCLUDES = includes/

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -I includes

RESET = \033[0m
GREEN = \033[0;92m
RED = \033[0;91m
MAGENTA = \033[0;95m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) ready !$(RESET)"

%.o: %.cpp $(INCLUDES)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling $<...$(RESET)"

clean:
	@rm -f $(OBJS)
	@echo "$(RED)Cleaning...$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(MAGENTA)Full clean$(RESET)"

re: fclean all

.PHONY: all clean fclean re