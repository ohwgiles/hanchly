
# Fetch Jun Da Frequency List tables. Tables are provided
# for research and teaching/learning purposes; commercial
# use must have prior written permission. See COPYING
file(DOWNLOAD "http://lingua.mtsu.edu/chinese-computing/statistics/char/download.php?Which=MO"
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_modern.csv
		SHOW_PROGRESS)
file(DOWNLOAD "http://lingua.mtsu.edu/chinese-computing/statistics/char/download.php?Which=CL"
		${CMAKE_CURRENT_BINARY_DIR}/jun_da_freq_classical.csv
		SHOW_PROGRESS)

# Fetch CC-CEDICT. CC-CEDICT is licensed under the CC-BY-SA 3.0
# See COPYING
file(DOWNLOAD "http://www.mdbg.net/chindict/export/cedict/cedict_1_0_ts_utf-8_mdbg.txt.gz"
		${CMAKE_CURRENT_BINARY_DIR}/cedict_1_0_ts_utf-8_mdbg.txt.gz
		SHOW_PROGRESS)

# Fetch the Unihan databese. Provided as-is by the Unicode
# Consortium. See COPYING
file(DOWNLOAD "http://www.unicode.org/Public/UCD/latest/ucd/Unihan.zip"
		${CMAKE_CURRENT_BINARY_DIR}/Unihan.zip
		SHOW_PROGRESS)
