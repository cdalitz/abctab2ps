/*
 * functions that are different on different systems
 */

#ifdef WIN32
  #include <windows.h>
  #include <lmaccess.h>
  #include <lmapibuf.h>
#else
  #include <sys/types.h>
  #include <unistd.h>
  #include <pwd.h>
#endif

#include <string>
using namespace std;

/* get real user name info */
string get_real_user_name()
{
  string user;
#ifdef WIN32
  char tmp[512];
  DWORD nchars = sizeof(tmp);
  if (GetUserName(tmp,&nchars)) {
	WCHAR wuser[512];
	char fullname[512];
	USER_INFO_2 *ui;
	MultiByteToWideChar(CP_ACP, 0, tmp, strlen(tmp)+1, wuser, sizeof(wuser)/sizeof(wuser[0]));
	if (NetUserGetInfo(NULL, (LPWSTR)&wuser, 2, (LPBYTE*) &ui)) {
	  // Error: use login name instead
	  user = tmp;
	} else {
	  WideCharToMultiByte(CP_ACP, 0, ui->usri2_full_name, -1, fullname, sizeof(fullname), NULL, NULL);
	  if (*fullname) {
		user = fullname;
	  } else {
		user = tmp;
	  }
	  NetApiBufferFree(ui);
	}
  } else {
    user = "";
  }
#else
  uid_t userid = getuid();
  struct passwd* user_info = getpwuid(userid);
  if (!user_info) {
    user = "";
  } else {
    user = user_info->pw_gecos;
  }
#endif
  return user;
}
