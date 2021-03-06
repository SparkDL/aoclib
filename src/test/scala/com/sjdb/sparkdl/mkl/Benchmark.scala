package com.sjdb.sparkdl.mkl

import com.intel.analytics.bigdl.mkl.MKL
import com.sjdb.sparkdl.utils.Utils
import com.sjdb.sparkdl.mkl.ACLMKL

import scala.util.Random

object Benchmark {
  def main(args: Array[String]): Unit = {
    sgemm()
  }

  def sgemm(): Unit = {
    val m = 4096
    val n = 4096
    val k = 4096
    val alpha = 1f
    val beta = 1f
    val a = Array.fill(m * k)(Random.nextFloat)
    val b = Array.fill(k * n)(Random.nextFloat)
    var c = Array.fill(m * n)(Random.nextFloat)

    var time:Double = 1.0

    MKL.vsgemm('n', 'n', m, n, k, alpha,
      a, 0, m,
      b, 0, k,
      beta, c, 0, m)

    ACLMKL.vsgemm('n', 'n', m, n, k, alpha,
      a, 0, m,
      b, 0, k,
      beta, c, 0, m)

    MKL.setNumThreads(8)
    time = Utils.executeTime(
      () => MKL.vsgemm('n', 'n', m, n, k, alpha,
        a, 0, m,
        b, 0, k,
        beta, c, 0, m),
      "mkl-sgemm",
      10)

    val mklGflops = Utils.gflops(4096, time)
    println(f"MKL: $mklGflops gflops")

    time = Utils.executeTime(
      () => ACLMKL.vsgemm('n', 'n', m, n, k, alpha,
        a, 0, m,
        b, 0, k,
        beta, c, 0, m),
      "aclmkl-sgemm",
      10)

    val aclmklGflops = Utils.gflops(4096, time)
    println(f"ACLMKL: $aclmklGflops gflops")
  }
}
