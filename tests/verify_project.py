"""Run final integration checks against a supplied built executable (Python 3)."""
from pathlib import Path
import argparse
import json
import re
import subprocess
import tempfile

SAMPLE = 'John Deere\n3\nC Programming\n85\nPython\n92\nComputer Architecture\n68\n'

def verify(executable, artifacts):
    results = []
    artifacts.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='grade-tests-') as temporary:
        work = Path(temporary)

        def execute(data, directory=work):
            return subprocess.run([str(executable)], input=data, text=True,
                                  capture_output=True, timeout=10, cwd=directory)

        def success(label, data, expected=()):
            result = execute(data)
            assert result.returncode == 0, (label, result.stderr, result.stdout)
            report = (work / 'student_report.txt').read_text()
            assert report in result.stdout, label + ': screen/file mismatch'
            assert 'Report saved to student_report.txt' in result.stdout
            for text in expected:
                assert text in report, (label, text)
            results.append('PASS: ' + label)
            return result, report

        result, report = success('Assignment example and identical saved report', SAMPLE,
            ('Average Grade: 3.67', 'Average Score: 81.67%', 'Overall Grade: 4'))
        (artifacts / 'sample_console.txt').write_text(result.stdout, encoding='utf-8')
        (artifacts / 'sample_input.txt').write_text(SAMPLE, encoding='utf-8')
        (artifacts / 'sample_report.txt').write_text(report, encoding='utf-8')
        header = next(line for line in report.splitlines() if line.startswith('Subject '))
        score_right = header.index('Score') + len('Score')
        for name in ('C Programming', 'Python', 'Computer Architecture'):
            row = next(line for line in report.splitlines() if line.startswith(name))
            assert row.index('%') == score_right - 1 and len(row) == len(header)
        results.append('PASS: numeric columns and table separators align')

        for score, grade in ((0, 0), (49, 0), (50, 1), (59, 1), (60, 2), (69, 2),
                             (70, 3), (79, 3), (80, 4), (89, 4), (90, 5), (100, 5)):
            success(f'Grade boundary {score} -> {grade}', f'Alice\n1\nMath\n{score}\n',
                    (f'Average Grade: {grade:.2f}', f'Overall Grade: {grade}'))
        success('Fractional average is not rounded before grading',
                'Alice\n2\nMath\n49\nScience\n50\n',
                ('Average Grade: 0.50', 'Average Score: 49.50%', 'Overall Grade: 0'))

        for invalid in ('', ' \t ', 'abc', '3.5', '3junk', '0', '-1', '1001', '9' * 400, '3\x00junk'):
            success('Invalid subject count rejected: ' + repr(invalid[:16]),
                    f'Alice\n{invalid}\n1\nMath\n85\n', ('Subjects: 1',))
        for invalid in ('', 'text', '5.5', '-1', '101', '9' * 400, '85\x00junk'):
            success('Invalid score rejected: ' + repr(invalid[:16]),
                    f'Alice\n1\nMath\n{invalid}\n100\n', ('Average Score: 100.00%',))
        for invalid in ('', '   ', 'X' * 400, 'Bad\x00Name', 'Bad\tName'):
            success('Invalid subject name rejected: ' + repr(invalid[:16]),
                    f'Alice\n1\n{invalid}\nMath\n50\n', ('Math', 'Average Score: 50.00%'))
        success('Whitespace around student name is trimmed',
                '\n  John Deere  \n1\nMath\n100\n', ('Student: John Deere\n',))
        success('127-byte names accepted with CRLF',
                'A' * 127 + '\r\n1\r\n' + 'B' * 127 + '\r\n100\r\n')
        success('Last score accepted without final newline', 'Alice\n1\nMath\n100')
        success('1000 subjects at maximum score', 'Alice\n1000\n' +
                ''.join(f'Subject {i}\n100\n' for i in range(1000)),
                ('Subjects: 1000', 'Average Score: 100.00%', 'Average Grade: 5.00'))

        for index, data in enumerate(('', 'Alice\n', 'Alice\n1\n', 'Alice\n1\nMath\n')):
            result = execute(data)
            assert result.returncode != 0 and 'Input ended' in result.stdout
            results.append(f'PASS: interrupted input at stage {index} exits without hanging')

        (work / 'student_report.txt').write_text('PREVIOUS REPORT')
        _, report = success('Existing report replaced', SAMPLE)
        assert 'PREVIOUS REPORT' not in report
        blocked = work / 'blocked'
        blocked.mkdir()
        (blocked / 'student_report.txt').mkdir()
        result = execute(SAMPLE, blocked)
        assert result.returncode != 0 and 'Could not open report file' in result.stderr
        assert 'Report saved' not in result.stdout
        results.append('PASS: report opening failure produces an error, no false success')
        (artifacts / 'save_failure.txt').write_text(result.stderr, encoding='utf-8')

    (artifacts / 'results.txt').write_text('\n'.join(results) + '\n', encoding='utf-8')
    (artifacts / 'results.json').write_text(json.dumps(results, indent=2), encoding='utf-8')
    print('\n'.join(results))
    print(f'\n{len(results)} checks passed.')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('executable', type=Path)
    parser.add_argument('--artifacts', type=Path, default=Path('build/test-results'))
    arguments = parser.parse_args()
    verify(arguments.executable.resolve(), arguments.artifacts.resolve())
