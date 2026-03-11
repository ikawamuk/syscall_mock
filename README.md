# syscall_mock

ユニットテスト用の「syscall/stdlib の失敗注入（failure injection）」を、リンカの `--wrap` で実現する小さな C ヘルパです。

## できること

- `malloc` / `free`：指定回数目の `malloc` を失敗させる、`free` の妥当性チェック、簡易リーク検知
- `read` / `open`：失敗させて `errno` を設定
- `exit`：プロセス終了せず `longjmp` でテストに戻す

- 他にも`__wrap_FUNCTOINNAME`を作成して任意のモックを作成してください

## 使い方（リンク）

`syscall_mock.c` をテスト用バイナリに一緒にリンクし、対象シンボルを `--wrap` します。

```bash
gcc -Wall -Wextra -Werror \
  test.c syscall_mock.c \
  -Wl,--wrap=malloc -Wl,--wrap=free -Wl,--wrap=read -Wl,--wrap=open -Wl,--wrap=exit
```

## 失敗注入の設定

`syscall_mock.h` で公開しているグローバルをテスト側から設定します。

- `err_syscall`：失敗させたい対象（`NOTHING`, `MALLOC`, `READ`, `OPEN`）
- `_wrap_errno_`：失敗時にセットする `errno`（例: `ENOMEM`, `EIO`, `ENOENT`）
- `MALLOC_FAIL_COUNT`：何回目の `malloc` を失敗させるか（コンパイル時マクロ）

例（2回目の `malloc` を `ENOMEM` で失敗させる）:

```bash
gcc test.c syscall_mock.c -DMALLOC_FAIL_COUNT=2 \
  -Wl,--wrap=malloc -Wl,--wrap=free
```

## 便利関数

- `bool is_memory_remain(void)`：`malloc` したものが残っていれば `true`

## `exit` をテストで捕まえる例

`exit` を呼ぶ関数をテストしたいときは `setjmp/longjmp` を使います。

```c
if (setjmp(g_test_jump_env) == 0) {
    /* ここで exit() する関数を呼ぶ */
} else {
    /* ここに戻ってくる。終了コードは g_last_exit_status */
}
```
