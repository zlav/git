#include "builtin.h"
#include "cache.h"
#include "config.h"
#include "parse-options.h"
#include "sequencer.h"

static const char * const builtin_rebase_helper_usage[] = {
	N_("git rebase--helper [<options>]"),
	NULL
};

int cmd_rebase__helper(int argc, const char **argv, const char *prefix)
{
	struct replay_opts opts = REPLAY_OPTS_INIT;
	int keep_empty = 0;
	enum {
		CONTINUE = 1, ABORT, MAKE_SCRIPT, SHORTEN_SHA1S, EXPAND_SHA1S,
		CHECK_TODO_LIST, SKIP_UNNECESSARY_PICKS, REARRANGE_SQUASH
	} command = 0;
	struct option options[] = {
		OPT_BOOL(0, "ff", &opts.allow_ff, N_("allow fast-forward")),
		OPT_BOOL(0, "keep-empty", &keep_empty, N_("keep empty commits")),
		OPT_CMDMODE(0, "continue", &command, N_("continue rebase"),
				CONTINUE),
		OPT_CMDMODE(0, "abort", &command, N_("abort rebase"),
				ABORT),
		OPT_CMDMODE(0, "make-script", &command,
			N_("make rebase script"), MAKE_SCRIPT),
		OPT_CMDMODE(0, "shorten-ids", &command,
			N_("shorten SHA-1s in the todo list"), SHORTEN_SHA1S),
		OPT_CMDMODE(0, "expand-ids", &command,
			N_("expand SHA-1s in the todo list"), EXPAND_SHA1S),
		OPT_CMDMODE(0, "check-todo-list", &command,
			N_("check the todo list"), CHECK_TODO_LIST),
		OPT_CMDMODE(0, "skip-unnecessary-picks", &command,
			N_("skip unnecessary picks"), SKIP_UNNECESSARY_PICKS),
		OPT_CMDMODE(0, "rearrange-squash", &command,
			N_("rearrange fixup/squash lines"), REARRANGE_SQUASH),
		OPT_END()
	};

	git_config(git_default_config, NULL);

	opts.action = REPLAY_INTERACTIVE_REBASE;
	opts.allow_ff = 1;
	opts.allow_empty = 1;

	argc = parse_options(argc, argv, NULL, options,
			builtin_rebase_helper_usage, PARSE_OPT_KEEP_ARGV0);

	if (command == CONTINUE && argc == 1)
		return !!sequencer_continue(&opts);
	if (command == ABORT && argc == 1)
		return !!sequencer_remove_state(&opts);
	if (command == MAKE_SCRIPT && argc > 1)
		return !!sequencer_make_script(keep_empty, stdout, argc, argv);
	if (command == SHORTEN_SHA1S && argc == 1)
		return !!transform_todo_ids(1);
	if (command == EXPAND_SHA1S && argc == 1)
		return !!transform_todo_ids(0);
	if (command == CHECK_TODO_LIST && argc == 1)
		return !!check_todo_list();
	if (command == SKIP_UNNECESSARY_PICKS && argc == 1)
		return !!skip_unnecessary_picks();
	if (command == REARRANGE_SQUASH && argc == 1)
		return !!rearrange_squash();
	usage_with_options(builtin_rebase_helper_usage, options);
}
