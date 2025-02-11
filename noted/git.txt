git init
git add .
git commit -m ""
git remote add origin REMOTE-URL
git remote -v
git push -u origin main
force-push:
  git push -u origin main -f

# Reference for adding locally hosted code to GitHub
help-github-import:
  @echo "Refer to these instructions for adding locally hosted code to GitHub:"
  @echo "https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github".PHONY: init add commit remote-add remote-verify push force-push help-github-import

When using sftp to remote to ubuntu server
- need using ssh to clone repo
- on ubuntu server
  ssh-keygen
  cat /home/huynh/.ssh/id_ed25519.pub
  -> get key, add to here on github https://github.com/settings/ssh/new
  git clone repo ssh
  -> done