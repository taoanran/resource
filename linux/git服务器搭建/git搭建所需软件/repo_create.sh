#本地仓库目录
local_repository=/home/taoanran/work/git/amlogicM3_0131/ics

#gitignore_repository=/home/linwei/git-repository-tools/gitignore

#服务器的仓库
remote_repository=git@192.168.0.112:amlogic-ics

#提交到服务器新的远程分支
#命名规则是ics-sunniwell-date ,date是imx6给的sdk版本日期
branch_repository=amlogic-ics-hansun-0131
#branch_repository=master

git_create()
{
    gitname=`basename $1`
    echo "$gitname:$local_repository/$1::$remote_repository/$2.git"
    cd $local_repository/$1
    if [ -d .git ] ;then
	echo "rm -rf .git*"
	rm -rf .git
    fi

    git init;
    if [ -d .git ] ;then
	echo "rm -rf .gitignore"
	rm -rf .gitignore
    fi
    git add . ;
    git commit -am "create project $gitname";
    git checkout -b  $branch_repository;
    git remote add origin $remote_repository/$2.git;
    git push origin $branch_repository;
}
git_build()
{
cat $1 | while read line
do 
    elemval=$(echo $line | sed -n -e '/<project path=/p' | sed -e 's/<project path=//g' | sed -e 's/name=//g' | sed -e 's/\"//g' | sed -e 's/\/>//g' | sed -e 's/>//g')
    local_dir=$( echo $elemval |cut -d ' ' -f 1)
    remote_dir=$( echo $elemval |cut -d ' ' -f 2)
    if [ "$local_dir" != "" ] && [ "$remote_dir" != "" ];then
#        echo $local_dir $remote_dir
        git_create $local_dir $remote_dir
    fi      
done    
}
manifestfile=$(echo $1 | sed -n -e '/default.xml/p')
if [ "$manifestfile" != "" ] ; then
#	echo "git build"
	git_build $1 
else
	echo "usage:./repo_create default.xml"
fi
exit


