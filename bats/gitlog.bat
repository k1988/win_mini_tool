@rem 以单行格式显示git日志。如：e4e65f8 U k1988  test sign
@rem git log --pretty="format:%%h %%G? %%aN  %%s" -10
git log --pretty="format:%%h %%ad %%aN  %%s" --date="format:%%Y%%m%%d %%H:%%M:%%S" -20