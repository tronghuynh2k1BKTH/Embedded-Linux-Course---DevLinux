```markdown
# Git Commands

git init
git add .
git commit -m ""
git remote add origin REMOTE-URL
git remote -v
git push -u origin main
```

## Force Push

```sh
git push -u origin main -f
```

## Reference for Adding Locally Hosted Code to GitHub

```sh
help-github-import:
  @echo "Refer to these instructions for adding locally hosted code to GitHub:"
  @echo "https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github"

init add commit remote-add remote-verify push force-push help-github-import
```

# Using SFTP to Remote to Ubuntu Server

- Need to use SSH to clone the repo
- On Ubuntu server:

  ```sh
  ssh-keygen
  cat /home/huynh/.ssh/id_ed25519.pub
  ```

  - Get the key and add it to GitHub: [Add SSH Key](https://github.com/settings/ssh/new)

  ```sh
  git clone repo ssh
