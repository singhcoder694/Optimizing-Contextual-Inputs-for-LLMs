# Optimizing Contextual Inputs for LLMs

**Summary:** This project improves LLM responses by programmatically generating focused context from a large textual corpus and using information-retrieval techniques (tries, TF–IDF, frequency priors and substring search) to rank and supply the most relevant excerpts as prompts to a language model.

**Highlights / Skills demonstrated**

- Languages: C++ (core IR engine), Python (OpenAI/LLM integration)
- Information Retrieval: Trie-based word counts, TF–IDF scoring, top-k ranking
- Search algorithms: efficient substring search over documents (Boyer–Moore style heuristics)
- Prompt engineering: automatic context extraction and query construction for LLMs
- Systems: large-corpus preprocessing, file I/O, and simple orchestration between C++ and Python

## Project Goals

- Generate compact, high-quality context from a large corpus so an LLM receives only the most relevant excerpts for a question.
- Use corpus-level statistics (unigram frequencies) to bias selection toward informative words.

## Repository structure

- `tester.cpp` — Example driver: loads the corpus, inserts sentences into the system, times insertion, and demonstrates a sample query flow.
- `qna_tool.cpp`, `qna_tool.h` — Main orchestration class. Builds TF–IDF scores for query tokens, ranks paragraphs, and prepares the top-k paragraphs to query the LLM.
- `search.cpp`, `search.h` — Search engine that stores corpus paragraphs and supports substring search (returns paragraph metadata and offsets).
- `dict.cpp`, `dict.h` — Dictionary implementation using a Trie to store and query word frequencies across the corpus.
- `Node.h` — Linked-list node / small metadata structure used by the search / qna components (required by several modules).
- `api_call.py` — Simple Python script that calls the OpenAI ChatCompletion API to send the composed context + query and print the reply.
- `unigram_freq.csv` — Precomputed unigram frequency CSV used as priors when scoring words.
- `mahatma-gandhi-collected-works-volume-*.txt` — (not included in repo) the corpus files expected by `tester.cpp` and `qna_tool`.

## How it works (high level)

1. Corpus ingestion: `tester.cpp` reads lines from the corpus files. Each sentence is inserted into `QNA_tool` which forwards it to two subsystems:
   - `Dict` (Trie) — counts occurrences of each token in the whole corpus.
   - `SearchEngine` — stores paragraph-level text and supports substring search with metadata (book, page, paragraph, offset).
2. Unigram priors: `QNA_tool` loads `unigram_freq.csv` into a small trie (`gen`) so the system can downweight extremely common words.
3. Query processing: given a question, `QNA_tool` tokenizes and assigns weights to words (stopwords vs content words). For each token it collects TF and document-frequency-like signals to build TF–IDF scores per paragraph.
4. Ranking: paragraphs are scored by combining TF–IDF, word weights, and offset heuristics. The top paragraphs are selected within a configurable word budget.
5. LLM integration: selected paragraphs are written to `paragraph_*.txt` files and a `query.txt` is prepared; `api_call.py` is invoked to send context + question to an LLM (OpenAI ChatCompletion in current implementation).

## Design & algorithms (concise)

- Trie (`Dict`) for fast corpus-wide word counting and dictionary dumping.
- Search engine stores original sentences and supports exact substring search using a Boyer–Moore-like heuristic (bad-character shift table) for fast pattern matching across many sentences.
- TF–IDF style scoring is computed per query token across paragraphs; results are combined using custom weighting (including unigram priors) to form a contextual relevance score.

## Prerequisites

- A modern C++ compiler (g++ or clang) supporting C++11/17.
- Python 3 and `openai` Python package (only needed if you want to run the LLM call):

```bash
python3 -m pip install openai
```

Note: The project expects the Gandhi corpus files `mahatma-gandhi-collected-works-volume-<n>.txt` (1..98) and `unigram_freq.csv` to be present in the working directory. These files are not included in the repo.

## Build & run (example)

1. Compile the C++ project (example):

```bash
g++ -std=c++17 tester.cpp qna_tool.cpp dict.cpp search.cpp -O2 -o tester
```

2. Run the ingestion and sample query (this will prepare paragraph files and a `query.txt`):

```bash
./tester
```

3. The current flow uses `api_call.py` to call an LLM. To run that script directly (recommended so you can provide a real API key):

```bash
# after tester created paragraph_0.txt ... paragraph_k-1.txt and query.txt
python3 api_call.py <YOUR_OPENAI_API_KEY> <NUM_PARAGRAPHS> query.txt
```

## Security & keys

- Never commit your OpenAI API key. Pass it at runtime as shown above.

## How to adapt / run end-to-end

- If you want the C++ program to directly pass your real API key to `api_call.py`, either:
  - Modify `qna_tool.cpp`'s `query_llm` call to substitute a secure runtime-provided API key; or
  - Manually run `api_call.py` with the produced `query.txt` and paragraph files as shown above.

## Notes & limitations

- This codebase was implemented as a course project with the objective of demonstrating IR techniques integrated with an LLM. It is not production hardened.
- Memory: several data structures allocate large vectors (e.g., `kyah`) sized for the corpus used in the assignment; you may need to tune memory parameters for different corpora.
- The `api_call.py` example uses OpenAI's ChatCompletion — adapt the model name or request format to your preferred LLM/API.

## Recommended CV bullets

- Implemented a hybrid IR + LLM pipeline that programmatically constructs high-quality context for generative models using Trie-based indexing and TF–IDF ranking.
- Built a custom substring search engine and paragraph-ranking algorithm in C++, and integrated it with OpenAI's ChatCompletion via a small Python orchestration script.
- Demonstrated performance and corpus-level pruning using unigram priors and custom heuristics to reduce prompt size while preserving answer quality.

## Contact / attribution

- Author: (your name) — include contact/email on your CV or GitHub profile.
- Course: COL106 (Prof. Subodh Sharma)

If you want, I can also:

- Add a short `RUN.md` with step-by-step commands tailored to your system.
- Add example outputs and a short GIF or screenshots showing sample LLM responses.

---

Updated README to be recruiter-friendly and actionable. If you'd like, I can now create a `RUN.md`, embed an example output, or update `qna_tool.cpp` so it accepts the API key at runtime instead of the placeholder string.

# Optimizing-Contextual-Inputs-for-LLMs

Course Project (COL106) under Prof. Subodh Sharma: Enhancing open-source LLM input by refining prompts, algorithms, and context for improved responses. Employed prompt engineering techniques like Stemming, Lematization, and TF-IDF aid in this optimization process.
