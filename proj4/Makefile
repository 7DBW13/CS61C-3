default:
	java -jar Ida.jar -a tests/*.ida

test:
	java -jar Ida.jar -a tests/*.ida
	java -jar Ida.jar -t

check:
	python gen.py > tests/testcase1.hex
	python gen.py > tests/testcase2.hex
	python gen.py > tests/testcase3.hex
	python gen.py > tests/testcase4.hex
	python gen.py > tests/testcase5.hex
	java -jar Ida.jar -t
	java -jar Ida.jar -v
