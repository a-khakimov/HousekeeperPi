import Dependencies._

name := "HouseKeeperPi"

version := "0.1"

scalaVersion := "2.13.1"


ThisBuild / scalaVersion := "2.13.1"
ThisBuild / organization := "com.github.ainr"
ThisBuild / organizationName := "ainr"

lazy val root = (project in file("."))
  .settings(
    name := "Akka Http Skeleton",
    libraryDependencies ++= Seq(
      akkaHttp,
      akkaStream,
      akkaTestKit % Test,
      akkaHttpTestKit % Test,
      specs2Core % Test
    )
  )

libraryDependencies += "com.typesafe.akka" %% "akka-http-spray-json" % "10.1.11"
