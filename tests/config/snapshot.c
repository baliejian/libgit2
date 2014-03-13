#include "clar_libgit2.h"

void test_config_snapshot__create_snapshot(void)
{
	int32_t tmp;
	git_config *cfg, *snapshot;
	const char *filename = "config-ext-change";

	cl_git_mkfile(filename, "[old]\nvalue = 5\n");

	cl_git_pass(git_config_open_ondisk(&cfg, filename));

	cl_git_pass(git_config_get_int32(&tmp, cfg, "old.value"));
	cl_assert_equal_i(5, tmp);

	cl_git_pass(git_config_snapshot(&snapshot, cfg));

	/* Change the value on the file itself (simulate external process) */
	cl_git_mkfile(filename, "[old]\nvalue = 56\n");

	cl_git_pass(git_config_get_int32(&tmp, cfg, "old.value"));
	cl_assert_equal_i(5, tmp);

	cl_git_pass(git_config_refresh(cfg));

	cl_git_pass(git_config_get_int32(&tmp, cfg, "old.value"));
	cl_assert_equal_i(56, tmp);

	cl_git_pass(git_config_get_int32(&tmp, snapshot, "old.value"));
	cl_assert_equal_i(5, tmp);
	
	git_config_free(snapshot);
	git_config_free(cfg);
}
