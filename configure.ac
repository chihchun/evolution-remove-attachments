AC_INIT([evolution-remove-attachments], [0.0.2], chihchun@kalug.linux.org.tw)
AC_CONFIG_SRCDIR(src)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

AM_MAINTAINER_MODE

dnl make sure we keep ACLOCAL_FLAGS around for maintainer builds to work
AC_SUBST(ACLOCAL_AMFLAGS, "\${ACLOCAL_FLAGS}")

AC_PROG_INTLTOOL([0.21])

AC_PROG_CC
AC_REQUIRE_CPP
AC_PATH_PROG(GPP, g++, no)
if test "x$GPP" = "xno"; then
	AC_MSG_ERROR(C++ Compiler (g++) not installed)
fi
#AC_PROG_CXXCPP
AC_ISC_POSIX
AC_HEADER_STDC
AM_DISABLE_STATIC
AM_PROG_LIBTOOL
dnl GNOME_COMPILE_WARNINGS

AC_MSG_CHECKING(Evolution version)
dnl is this a reasonable thing to do ?
EVOLUTION_VERSION=`pkg-config --modversion evolution-shell 2>/dev/null`
if test -n "$EVOLUTION_VERSION"; then
	EVOLUTION_BASE_VERSION=$EVOLUTION_VERSION
	EVOLUTION_BASE_VERSION_S=""
	EVOLUTION_EXEC_VERSION=`pkg-config --variable=execversion evolution-shell 2>/dev/null`
	if test -n "$EVOLUTION_EXEC_VERSION"; then
		break;
	else
		dnl we need major minor here
		EVOLUTION_EXEC_VERSION=$EVOLUTION_BASE_VERSION
		break;
	fi
else
	evo_versions='12 11 10 8 6 4'
	for i in $evo_versions; do
		EVOLUTION_VERSION=`pkg-config --modversion evolution-shell-2.$i 2>/dev/null`
		if test -n "$EVOLUTION_VERSION"; then
			EVOLUTION_BASE_VERSION=2.$i
			EVOLUTION_BASE_VERSION_S="-"$EVOLUTION_BASE_VERSION
			dnl this might be required for devel version
			EVOLUTION_EXEC_VERSION=`pkg-config --variable=execversion evolution-shell-2.$i 2>/dev/null`
			if test -n "$EVOLUTION_EXEC_VERSION"; then
				break;
			else
				EVOLUTION_EXEC_VERSION=$EVOLUTION_BASE_VERSION
				break;
			fi
			break;
		else
			continue;
		fi
	done
	if test -z "$EVOLUTION_VERSION"; then
       		AC_MSG_ERROR(Evolution development libraries not installed)
	fi
fi
AC_SUBST(EVOLUTION_VERSION)
AC_MSG_RESULT($EVOLUTION_VERSION)
AC_SUBST(EVOLUTION_BASE_VERSION)
AC_SUBST(EVOLUTION_EXEC_VERSION)
evolution_exec_int="$(echo "$EVOLUTION_EXEC_VERSION" | $AWK -F . '{print [$]1 * 10000 + [$]2 * 100 + [$]3}')"
if test "$evolution_exec_int" -ge "21100"; then
	AC_DEFINE_UNQUOTED(EVOLUTION_2_12,1, [evolution mail 2.12 present])
	AC_SUBST(EVOLUTION_2_12)
fi
AC_SUBST(evolution_exec_int)

MINOR_VERSION=`echo $EVOLUTION_EXEC_VERSION|cut -d. -f2|awk '{print 1000 * $1}'`
AC_SUBST(MINOR_VERSION)

dnl base packages and versions
LIBGTK_REQUIRED=2.4.0
LIBGNOME_REQUIRED=2.7.0
LIBGNOMEUI_REQUIRED=2.7.0
EVOLUTION_REQUIRED=2.4.1
LIBGLADE_REQUIREED=2.0

PKG_CHECK_MODULES(EVOLUTION_REMOVE_ATTACHMENTS_EPLUGIN,
  [libgnome-2.0 >= $LIBGNOME_REQUIRED dnl
   libgnomeui-2.0 >= $LIBGNOMEUI_REQUIRED dnl
   gtk+-2.0 >= $LIBGTK_REQUIRED dnl
   evolution-plugin$EVOLUTION_BASE_VERSION_S >= $EVOLUTION_REQUIRED dnl
   libglade-2.0 >= $LIBGLADE_REQUIREED dnl
])

dnl Evolution plugin install directory
AC_ARG_WITH(plugin-install-dir, [  --with-plugin-install-dir=PATH path to evolution plugin directory])
if test "x$with_plugin_install_dir" = "x"; then
   	PLUGIN_INSTALL_DIR=`pkg-config --variable=plugindir evolution-plugin$EVOLUTION_BASE_VERSION_S`
   	if test "x$PLUGIN_INSTALL_DIR" = "x"; then
      		AC_MSG_ERROR(Unable to find plugin directory)
		break;
	fi
fi
AC_SUBST(PLUGIN_INSTALL_DIR)

GETTEXT_PACKAGE=evolution-remove-attachments
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [definition of GETTEXT_PACKAGE])
AM_GLIB_DEFINE_LOCALEDIR(GNOMELOCALEDIR)
LOCALEDIR=$localedir
AC_SUBST(LOCALEDIR)

SOEXT='.so'
AC_SUBST(SOEXT)

EVO_SERVER_RULE=$srcdir/server.mk
AC_SUBST_FILE(EVO_SERVER_RULE)

# Generated files from automake
AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT
