# Steps for the owner

Everything below needs you: your own details, your GitHub account, or your decision.
Run commands from the repo folder:

```
cd ~/Desktop/ALU_Projects/c-dynamic-engines
```

## 1. Look at the branches

The work is on three branches, one per project. `main` holds only the shared
helpers and docs.

| Branch | Project |
|---|---|
| `feat/academic-risk-engine` | Project 1 |
| `feat/text-mining-analyzer` | Project 2 |
| `feat/iot-callback-engine` | Project 3 |

```
git branch                                   # list branches
git switch feat/academic-risk-engine         # open one
git log --oneline main..HEAD                 # its commits
```

## 2. Student ID in the headers (done)

Each project prints `Student ID: NTIVUNWA250` (your GitHub username, since no
student number was available). To change it, edit `STUDENT_ID` in
`include/config.h` on each project's branch and commit.

## 3. Run the tests

On each branch:

```
make test
```

Each project prints `N checks, 0 failed`, `integration: all checks passed`, and
its performance timings. Tests use AddressSanitizer, so a memory leak or overflow
fails the run.

## 4. Build, run and make the sample outputs

Project 1, on its branch:

```
cd project1_academic_risk
make
./bin/academic_engine                                              # try it by hand
./bin/academic_engine < samples/demo_input.txt > samples/sample_output.txt
cd ..
```

Project 2:

```
cd project2_text_mining
make
./bin/text_analyzer < samples/demo_input.txt > samples/sample_output.txt
cd ..
```

Project 3:

```
cd project3_iot_monitor
make
./bin/iot_monitor --seed 42 < samples/demo_input.txt > samples/sample_output.txt
cd ..
```

Open each `sample_output.txt` and check it. The scripted sessions already show
adding and removing records, duplicates being rejected, bad input, every menu
option, and (for Project 3) a reading being skipped for a removed device. Commit
the outputs on their branches:

```
git add project1_academic_risk/samples/sample_output.txt
git commit -m "Add project 1 sample output"
```

## 5. (Optional) Use your own data

- **Project 1**: change the 12 starting students in `src/dataset.c`. Keep between
  10 and 15. If you change grades, `tests/integration.sh` checks a few exact
  names, so run `make test` and update the expected lines.
- **Project 2**: the built-in paragraph is *Moby-Dick* (public domain). To use
  another one (150+ words), either paste it with menu option 1 at run time, or
  replace `SAMPLE_TITLE`, `SAMPLE_DETAILS` and `SAMPLE_PARAGRAPH` in
  `src/sample_text.c`. Write dashes as `--` and keep it ASCII. The integration test
  and perf test check the Moby-Dick numbers (201 words, 134 distinct), so update
  them too.
- **Project 3**: the starting devices are in `src/engine.c`, and the ranges and
  limits are in `src/device.c`.

## 6. Read the custom analysis write-ups

They explain the formula and algorithm, with worked examples. Read them so you can
explain them in your own words:

- `project1_academic_risk/docs/custom_analysis.md`: Academic Risk Index
- `project2_text_mining/docs/custom_analysis.md`: Lexical Diversity Score
- `project2_text_mining/docs/text_rules.md`: punctuation, apostrophe and hyphen rules
- `project3_iot_monitor/docs/custom_callback.md`: Sensor Drift Detector

## 7. Create the GitHub repo and push

You are logged in to `gh` as NTIVUNWA250. Make it private first; you can make it
public later.

```
gh repo create c-dynamic-engines --private --source . --remote origin
git push -u origin main
git push -u origin feat/academic-risk-engine feat/text-mining-analyzer feat/iot-callback-engine
```

Without `gh`:
1. Create an empty repo on github.com (no README).
2. Run `git remote add origin https://github.com/NTIVUNWA250/c-dynamic-engines.git`.
3. Run the two `git push` lines above.

## 8. Merge into main (your call)

Open one pull request per branch on GitHub, review it, and merge with
**"Create a merge commit"** (not squash). Or from the terminal:

```
gh pr create --base main --head feat/academic-risk-engine --title "Project 1: academic risk engine" --body "Adds project 1."
```

After merging, run `git switch main && git pull && make test` to check everything
together.

## Notes

- **Windows**: use WSL (Ubuntu) and run the same commands there.
- **Logs**: they are hidden by default. Run `APP_LOG=info ./bin/<program>` to see
  allocations, resizes and alerts on stderr.
- **Remove builds**: run `make clean` (in a project or at the root).
