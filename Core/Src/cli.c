/*
 * MIT License
 *
 * Copyright (c) 2019 Sean Farrelly
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * File        cli.c
 * Created by  Sean Farrelly
 * Version     1.0
 *
 */

/*! @file cli.c
 * @brief Implementation of command-line interface.
 */
#include "cli.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static volatile uint8_t buf[MAX_BUF_SIZE]; /* CLI Rx byte-buffer */
static volatile uint8_t *buf_ptr;	   /* Pointer to Rx byte-buffer */

static uint8_t cmd_buf[MAX_BUF_SIZE]; /* CLI command buffer */
static volatile cli_state_t cmd_state;

const char cli_unrecog[] = "%02d Fail\r\n";

/*!
 * @brief This internal API prints a message to the user on the CLI.
 */
static void cli_print(cli_t *cli, const char *msg);

/*!
 * @brief This API initialises the command-line interface.
 */
cli_status_t cli_init(cli_t *cli)
{
	/* Set buffer ptr to beginning of buf */
	buf_ptr = buf;
	cmd_state = CLI_MSG_ADDR;
	cli->address = 0xFF;

	return CLI_OK;
}

/*!
 * @brief This API deinitialises the command-line interface.
 */
cli_status_t cli_deinit(cli_t *cli)
{
	return CLI_OK;
}

void cli_set_address(cli_t *cli, uint8_t address)
{
	cli->address = address;

	return;
}

static bool cli_verify_address(cli_t *cli, uint8_t address)
{
	return (cli->address == address);
}

/*! @brief This API must be periodically called by the user to process and
 * execute any commands received.
 */
cli_status_t cli_process(cli_t *cli)
{
	if(cmd_state != CLI_MSG_PEND)
		return CLI_IDLE;

	uint8_t argc = 0;
	char *argv[30];

	/* Get the first token (cmd name) */
	argv[argc] = strtok(cmd_buf, " ");

	/* Walk through the other tokens (parameters) */
	while((argv[argc] != NULL) && (argc < 30)) {
		argv[++argc] = strtok(NULL, " ");
	}

	/* Search the command table for a matching command, using argv[0]
	 * which is the command name. */
	for(size_t i = 0; i < cli->cmd_cnt; i++) 
	{
		if(strcmp(argv[0], cli->cmd_tbl[i].cmd) == 0) 
		{
			/* Found a match, execute the associated function. */
			cli_status_t return_value = cli->cmd_tbl[i].func(argc, argv);
			cmd_state = CLI_MSG_ADDR;
			return return_value;
		}
	}

	/* Command not found */
	sprintf(cmd_buf, cli_unrecog, cli->address);
	cli_print(cli, cmd_buf);

	cmd_state = CLI_MSG_ADDR;
	return CLI_E_CMD_NOT_FOUND;
}

/*!
 * @brief This API should be called from the devices interrupt handler whenever
 * a character is received over the input stream.
 */
cli_status_t cli_put(cli_t *cli, char c)
{
	switch(c) 
	{
		case CMD_TERMINATOR:
		{
			if(CLI_MSG_CMD == cmd_state) 
			{
				*buf_ptr = '\0';      	/* Terminate the msg and reset the msg ptr.      */
				strcpy(cmd_buf, buf); 	/* Copy string to command buffer for processing. */
				cmd_state = CLI_MSG_PEND;
				buf_ptr = buf; 			/* Reset buf_ptr to beginning.                   */
			}
			else
			{
				cmd_state = CLI_MSG_ADDR;
				buf_ptr = buf; 			/* Reset buf_ptr to beginning.                   */
				return CLI_E_INVALID_ARGS;
			}
			break;
		}
		case CMD_NEWLINE:
		{
			/* Just ignore */
			break;
		}
		default:
		{
			/* Normal character received, add to buffer. */
			if((buf_ptr - buf) < MAX_BUF_SIZE)
			{
				*buf_ptr++ = c;
				if (CLI_MSG_ADDR == cmd_state)
				{
					if (!isdigit(c))
					{
						buf_ptr = buf;
						return CLI_E_INVALID_ARGS;		
					}

					if (ADDR_SIZE == buf_ptr - buf)
					{
						*buf_ptr = '\0';
						uint8_t addr = atoi(buf);
						if ((addr >= ADDR_MIN) && (addr <= ADDR_MAX) && cli_verify_address(cli, addr))
						{
							buf_ptr = buf;
							cmd_state = CLI_MSG_CMD;
						}
					}
				}
			}
			else
			{
				buf_ptr = buf;
				cmd_state = CLI_MSG_ADDR;
				return CLI_E_BUF_FULL;
			}
			break;
		}
	}

	return;
}

/*!
 * @brief Print a message on the command-line interface.
 */
static void cli_print(cli_t *cli, const char *msg)
{
	/* Temp buffer to store text in ram first */
	char buf[50];

	strcpy(buf, msg);
	cli->println(buf);
}
